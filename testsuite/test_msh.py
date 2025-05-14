import subprocess
import time
import os
import select
import fcntl
import tempfile
import shutil
import difflib

# --- Configuration ---
MINISHELL_EXECUTABLE = '../minishell'  # Replace with the actual path to your compiled minishell
#SHELL_PROMPT = "🐚\033[38;5;199mconchinha\033[38;5;99m>\033[0m "          # The exact prompt your shell displays
SHELL_PROMPT = "minishell> "          # The exact prompt your shell displays
TIMEOUT_SECONDS = 2                   # How long to wait for output (increased slightly)
GET_EXIT_CODE_COMMAND = 'echo $?'     # Command to get the last exit code in your minishell (like bash's $?)
                                      # !! IMPORTANT: Change this if your shell uses a different mechanism !!
BASH_PATH = shutil.which('bash')      # Find bash executable

# --- Helper Function to Run Command in Bash ---
def run_in_bash(command, stdin_content=None, timeout=TIMEOUT_SECONDS):
    """
    Runs a command in bash and captures its output and exit code.

    Args:
        command (str): The command string to execute.
        stdin_content (str, optional): String to pass as stdin to the command.
        timeout (int): Timeout for the subprocess execution.

    Returns:
        tuple: (stdout_str, stderr_str, exit_code) or None if bash is not found or error occurs.
    """
    if not BASH_PATH:
        print("Error: Bash executable not found.")
        return None

    try:
        input_bytes = stdin_content.encode('utf-8') if stdin_content is not None else None
        # Using shell=True here is generally okay for testing controlled commands,
        # but be mindful of security implications if commands were user-provided.
        # We use it here so bash handles pipes and redirections directly from the command string.
        result = subprocess.run(
            [BASH_PATH, '-c', command], # Use -c to interpret the command string
            input=input_bytes,
            capture_output=True,
            timeout=timeout,
            text=True, # Decode stdout/stderr as text
            errors='replace' # Handle potential decoding errors
        )
        return result.stdout, result.stderr, result.returncode
    except subprocess.TimeoutExpired:
        print(f"Error: Bash command '{command}' timed out after {timeout} seconds.")
        return None, "Timeout Error", -1 # Indicate timeout
    except Exception as e:
        print(f"Error running command in bash: {e}")
        return None, str(e), -1 # Indicate other error

# --- Helper Function to Read Minishell Output ---
def read_shell_output(process, timeout=TIMEOUT_SECONDS):
    """
    Reads output from the minishell process's stdout and stderr until the prompt
    is detected or a timeout occurs. (Modified for slightly more robust reading)

    Args:
        process: The subprocess.Popen object representing the running shell.
        timeout: Max seconds to wait for the prompt.

    Returns:
        A tuple containing (stdout_output_str, stderr_output_str).
    """
    stdout_data = b""
    stderr_data = b""

    # Ensure streams are non-blocking
    try:
        fcntl.fcntl(process.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
        fcntl.fcntl(process.stderr.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
    except OSError as e:
        print(f"Warning: Could not set non-blocking mode on streams: {e}")
        # Proceed anyway, but reading might block

    end_time = time.time() + timeout
    prompt_bytes = SHELL_PROMPT.encode('utf-8')
    last_output_time = time.time()

    while time.time() < end_time:
        ready_to_read, _, _ = select.select([process.stdout, process.stderr], [], [], 0.1)

        output_received_this_cycle = False
        for stream in ready_to_read:
            try:
                chunk = stream.read()
                if chunk:
                    if stream == process.stdout:
                        stdout_data += chunk
                    else:
                        stderr_data += chunk
                    output_received_this_cycle = True
                    last_output_time = time.time() # Reset timer on receiving output
            except (IOError, OSError):
                pass # Stream might be closed or temporarily unavailable

        # More robust prompt detection: check if the *end* of stdout has the prompt
        if stdout_data.endswith(prompt_bytes):
             # Wait a tiny bit more to catch any trailing output after the prompt appears
             time.sleep(0.05)
             # Try one final read
             try:
                 chunk = process.stdout.read()
                 if chunk: stdout_data += chunk
             except (IOError, OSError): pass
             try:
                 chunk = process.stderr.read()
                 if chunk: stderr_data += chunk
             except (IOError, OSError): pass
             break # Prompt found

        # Add a small sleep if nothing was read to prevent high CPU usage
        if not output_received_this_cycle:
            # If no output for a while, maybe the process finished without a prompt?
            if time.time() - last_output_time > 0.5: # 0.5s idle threshold
                 # print("Debug: Potential idle timeout reached.")
                 break
            time.sleep(0.02)
        # else:
            # If we received output but no prompt yet, continue quickly
            # time.sleep(0.001)


    # Decode final output
    stdout_str = stdout_data.decode('utf-8', errors='replace')
    stderr_str = stderr_data.decode('utf-8', errors='replace')

    return stdout_str, stderr_str

# --- Helper to Run a Command in Minishell and Get Exit Code ---
def run_minishell_command(process, command, get_exit_code=True):
    """
    Sends a command to the running minishell process, reads output,
    and optionally tries to get the exit code using GET_EXIT_CODE_COMMAND.

    Args:
        process: The subprocess.Popen object for the minishell.
        command (str): The command to send.
        get_exit_code (bool): Whether to attempt fetching the exit code.

    Returns:
        tuple: (stdout_str, stderr_str, exit_code_int or None)
               Exit code is None if not retrieved or parsing fails.
    """
    # Clear any previous output before sending command
    # read_shell_output(process, timeout=0.1) # Short timeout to clear buffer

    # Send the main command
    try:
        process.stdin.write(f"{command}\n".encode('utf-8'))
        process.stdin.flush()
    except (OSError, ValueError) as e:
        print(f"Error sending command '{command}' to minishell: {e}")
        return "", f"Error sending command: {e}", None

    # Read the output of the main command
    stdout, stderr = read_shell_output(process)

    # --- Clean the output ---
    # Remove the prompt itself from the end of stdout
    if stdout.endswith(SHELL_PROMPT):
        stdout_cleaned = stdout[:-len(SHELL_PROMPT)].rstrip()
    else:
        # Handle cases where prompt might be missing (e.g., after exit)
        # Or if the command output itself ends with the prompt string by chance
        # This part might need refinement based on shell behavior
        stdout_cleaned = stdout.rstrip()
        if stdout_cleaned.endswith(SHELL_PROMPT.rstrip()): # Check without trailing space if prompt has one
             stdout_cleaned = stdout_cleaned[:-len(SHELL_PROMPT.rstrip())].rstrip()


    # --- Get Exit Code (if requested) ---
    exit_code = None
    if get_exit_code and GET_EXIT_CODE_COMMAND:
        try:
            process.stdin.write(f"{GET_EXIT_CODE_COMMAND}\n".encode('utf-8'))
            process.stdin.flush()
            ec_stdout, ec_stderr = read_shell_output(process)

            # The exit code should be the first line of ec_stdout, before the next prompt
            ec_stdout_cleaned = ec_stdout
            if ec_stdout_cleaned.endswith(SHELL_PROMPT):
                 ec_stdout_cleaned = ec_stdout_cleaned[:-len(SHELL_PROMPT)].rstrip()

            # Extract the number
            try:
                # Find the last number in the cleaned output
                potential_codes = [int(s) for s in ec_stdout_cleaned.split() if s.isdigit()]
                if potential_codes:
                    exit_code = potential_codes[-1]
                else:
                     print(f"Warning: Could not parse exit code from output: '{ec_stdout_cleaned}'")
            except ValueError:
                print(f"Warning: Could not parse exit code from output: '{ec_stdout_cleaned}'")
            except Exception as e:
                 print(f"Warning: Error parsing exit code: {e}")


            # Append stderr from exit code command (if any) to main stderr
            if ec_stderr:
                stderr += f"\n[Stderr from exit code cmd]: {ec_stderr}"

        except (OSError, ValueError) as e:
            print(f"Error sending exit code command to minishell: {e}")
            stderr += f"\nError sending exit code command: {e}"
        except Exception as e:
            print(f"Unexpected error getting exit code: {e}")
            stderr += f"\nUnexpected error getting exit code: {e}"

    return stdout_cleaned, stderr, exit_code


# --- Main Test Comparison Function ---
def run_comparison_test(test_description, command,
                        stdin_content=None,
                        expect_exact_stdout=False, # If True, requires exact match, otherwise substring
                        ignore_stderr=False,       # Useful if minishell/bash differ minorly on errors
                        output_file_to_check=None, # Filename created by redirection (e.g., 'out.txt')
                        input_file_content=None,   # Content for a file used with '<'
                        input_file_name=None):     # Name for the temp input file
    """
    Runs a command in both minishell and bash, comparing stdout, stderr,
    exit code, and optionally the content of a redirected output file or
    using a redirected input file.

    Args:
        test_description (str): A description of the test.
        command (str): The command to test.
        stdin_content (str, optional): Content for stdin.
        expect_exact_stdout (bool): Require exact stdout match vs substring.
        ignore_stderr (bool): Don't compare stderr.
        output_file_to_check (str, optional): Filename to compare after execution.
        input_file_content (str, optional): Content for temporary input file.
        input_file_name (str, optional): Name to use for the temporary input file within the command.

    Returns:
        bool: True if the test passes (outputs match), False otherwise.
    """
    print(f"\n--- Test: {test_description} ---")
    print(f"Command: {command}")

    bash_stdout, bash_stderr, bash_exit_code = None, None, -999
    ms_stdout, ms_stderr, ms_exit_code = None, None, -998
    bash_outfile_content, ms_outfile_content = None, None
    temp_dir = None
    original_dir = os.getcwd()
    passed = True

    try:
        # Create a temporary directory for test files
        temp_dir = tempfile.mkdtemp(prefix="minishell_test_")
        os.chdir(temp_dir) # Run commands within the temp dir

        # --- Prepare Input File (if needed) ---
        actual_input_filename = None
        if input_file_content is not None and input_file_name is not None:
            actual_input_filename = os.path.join(temp_dir, input_file_name)
            with open(actual_input_filename, 'w') as f:
                f.write(input_file_content)
            print(f"Created input file '{input_file_name}' with content.")

        # --- Run in Bash ---
        print("Running in Bash...")
        bash_result = run_in_bash(command, stdin_content)
        if bash_result is None:
            print("FAIL: Could not execute in Bash.")
            return False # Cannot compare
        bash_stdout, bash_stderr, bash_exit_code = bash_result
        print(f"Bash   -> exit={bash_exit_code}, stdout='{bash_stdout[:100]}...', stderr='{bash_stderr[:100]}...'")

        # Read output file content if needed (Bash)
        if output_file_to_check:
            try:
                with open(output_file_to_check, 'r') as f:
                    bash_outfile_content = f.read()
                print(f"Bash created '{output_file_to_check}' with content: '{bash_outfile_content[:100]}...'")
            except FileNotFoundError:
                print(f"Bash did not create expected output file '{output_file_to_check}'.")
                bash_outfile_content = None # Explicitly None if not found
            except Exception as e:
                 print(f"Error reading bash output file: {e}")
                 bash_outfile_content = f"Error reading file: {e}"


        # Clean up output file before running minishell if it exists
        if output_file_to_check and os.path.exists(output_file_to_check):
             os.remove(output_file_to_check)


        # --- Run in Minishell ---
        print("Running in Minishell...")
        process = None
        try:
            # Start the minishell process
            minishell_abs_path = os.path.join(original_dir, MINISHELL_EXECUTABLE) # Use absolute path
            if not os.path.exists(minishell_abs_path):
                 print(f"FAIL: Minishell executable not found at '{minishell_abs_path}'")
                 return False

            process = subprocess.Popen(
                [minishell_abs_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                bufsize=0, # Unbuffered
                cwd=temp_dir # Run minishell in the temp directory
            )

            # Wait for the initial prompt
            initial_stdout, initial_stderr = read_shell_output(process, timeout=TIMEOUT_SECONDS + 1) # Longer timeout for startup
            if SHELL_PROMPT not in initial_stdout:
                print(f"FAIL: Minishell initial prompt '{SHELL_PROMPT}' not found.")
                print(f"Initial stdout: {initial_stdout}")
                print(f"Initial stderr: {initial_stderr}")
                return False

            # Run the actual command and get exit code
            ms_stdout, ms_stderr, ms_exit_code = run_minishell_command(process, command)
            print(f"Minishell -> exit={ms_exit_code}, stdout='{ms_stdout[:100]}...', stderr='{ms_stderr[:100]}...'")


            # Read output file content if needed (Minishell)
            if output_file_to_check:
                try:
                    with open(output_file_to_check, 'r') as f:
                        ms_outfile_content = f.read()
                    print(f"Minishell created '{output_file_to_check}' with content: '{ms_outfile_content[:100]}...'")
                except FileNotFoundError:
                    print(f"Minishell did not create expected output file '{output_file_to_check}'.")
                    ms_outfile_content = None # Explicitly None if not found
                except Exception as e:
                    print(f"Error reading minishell output file: {e}")
                    ms_outfile_content = f"Error reading file: {e}"

            # Send exit command to minishell
            try:
                process.stdin.write(b'exit\n')
                process.stdin.flush()
            except (OSError, ValueError):
                pass # Ignore if already closed

        except FileNotFoundError:
            print(f"FAIL: Minishell executable '{MINISHELL_EXECUTABLE}' not found.")
            passed = False
        except Exception as e:
            print(f"FAIL: An unexpected error occurred during minishell execution: {e}")
            passed = False
        finally:
            # Ensure the process is terminated
            if process:
                process.terminate()
                try:
                    process.wait(timeout=1)
                except subprocess.TimeoutExpired:
                    print("Warning: Minishell process did not terminate gracefully, killing.")
                    process.kill()

        # --- Comparisons ---
        print("Comparing results...")

        # 1. Exit Code
        # Allow comparison only if minishell could retrieve an exit code
        if ms_exit_code is None:
             print("WARN: Could not retrieve exit code from minishell. Skipping exit code comparison.")
        elif bash_exit_code != ms_exit_code:
            print(f"FAIL: Exit codes differ. Bash={bash_exit_code}, Minishell={ms_exit_code}")
            passed = False
        else:
            print(f"PASS: Exit codes match ({bash_exit_code}).")

        # 2. Stdout
        # Normalize whitespace slightly (strip trailing) for comparison robustness
        bash_stdout_norm = bash_stdout.rstrip()
        ms_stdout_norm = ms_stdout.rstrip()

        if expect_exact_stdout:
            if bash_stdout_norm != ms_stdout_norm:
                print(f"FAIL: Stdout differs (exact match required).")
                print("--- Bash Stdout ---")
                print(bash_stdout_norm)
                print("--- Minishell Stdout ---")
                print(ms_stdout_norm)
                print("--- Diff ---")
                diff = difflib.unified_diff(bash_stdout_norm.splitlines(keepends=True),
                                            ms_stdout_norm.splitlines(keepends=True),
                                            fromfile='bash_stdout', tofile='ms_stdout')
                print(''.join(diff))
                passed = False
            else:
                print("PASS: Stdout matches exactly.")
        else: # Substring check (less strict)
             if bash_stdout_norm not in ms_stdout_norm:
                 print(f"FAIL: Expected stdout substring not found in minishell output.")
                 print(f"Expected contains: '{bash_stdout_norm}'")
                 print(f"Minishell stdout: '{ms_stdout_norm}'")
                 passed = False
             else:
                 print(f"PASS: Minishell stdout contains expected output.")


        # 3. Stderr
        if not ignore_stderr:
             # Normalize whitespace slightly (strip trailing)
             bash_stderr_norm = bash_stderr.rstrip()
             ms_stderr_norm = ms_stderr.rstrip()
             if bash_stderr_norm != ms_stderr_norm:
                 print(f"FAIL: Stderr differs.")
                 print("--- Bash Stderr ---")
                 print(bash_stderr_norm)
                 print("--- Minishell Stderr ---")
                 print(ms_stderr_norm)
                 print("--- Diff ---")
                 diff = difflib.unified_diff(bash_stderr_norm.splitlines(keepends=True),
                                             ms_stderr_norm.splitlines(keepends=True),
                                             fromfile='bash_stderr', tofile='ms_stderr')
                 print(''.join(diff))

                 passed = False
             else:
                 print("PASS: Stderr matches.")
        else:
             print("INFO: Stderr comparison ignored.")


        # 4. Output File Content
        if output_file_to_check:
            # Handle case where one created the file and the other didn't
            if bash_outfile_content is None and ms_outfile_content is None:
                 print(f"PASS: Neither shell created output file '{output_file_to_check}' (as expected?).")
            elif bash_outfile_content is None:
                 print(f"FAIL: Bash did not create '{output_file_to_check}', but minishell did.")
                 passed = False
            elif ms_outfile_content is None:
                 print(f"FAIL: Minishell did not create '{output_file_to_check}', but bash did.")
                 passed = False
            elif bash_outfile_content != ms_outfile_content:
                print(f"FAIL: Output file '{output_file_to_check}' contents differ.")
                print("--- Bash File Content ---")
                print(bash_outfile_content)
                print("--- Minishell File Content ---")
                print(ms_outfile_content)
                print("--- Diff ---")
                diff = difflib.unified_diff(bash_outfile_content.splitlines(keepends=True),
                                            ms_outfile_content.splitlines(keepends=True),
                                            fromfile='bash_file', tofile='ms_file')
                print(''.join(diff))

                passed = False
            else:
                print(f"PASS: Output file '{output_file_to_check}' contents match.")


        return passed

    except Exception as e:
        import traceback
        print(f"FAIL: Unexpected exception in test runner: {e}")
        traceback.print_exc()
        return False
    finally:
        # --- Cleanup ---
        # Return to original directory
        os.chdir(original_dir)
        # Remove temporary directory
        if temp_dir and os.path.exists(temp_dir):
            try:
                shutil.rmtree(temp_dir)
                # print(f"Cleaned up temp directory: {temp_dir}")
            except Exception as e:
                print(f"Warning: Could not remove temporary directory {temp_dir}: {e}")
        print("-" * (len(test_description) + 12))


# --- Run Tests ---
if __name__ == "__main__":
    """
    process = subprocess.Popen(
            "/home/marcampo/projects/minishell/minishell",
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            bufsize=0)

    initial_stdout, initial_stderr = read_shell_output(process, timeout=TIMEOUT_SECONDS + 1) # Longer timeout for startup
    if SHELL_PROMPT not in initial_stdout:
        print(f"FAIL: Minishell initial prompt '{SHELL_PROMPT}' not found.")
        print(f"Initial stdout: {initial_stdout}")
        print(f"Initial stderr: {initial_stderr}")


    ms_stdout, ms_stderr, ms_exit_code = run_minishell_command(process, "echo\
    poooop")
    print(f"Minishell -> exit={ms_exit_code}, stdout='{ms_stdout[:100]}...', stderr='{ms_stderr[:100]}...'")
    """
    if not BASH_PATH:
        print("FATAL: Cannot run tests without bash. Please install bash or check PATH.")
        exit(1)

    test_results = {} # Use dict for named results

    # === Basic Commands ===
    test_results["echo"] = run_comparison_test(
        "Simple Echo", "echo hello world", expect_exact_stdout=True
    )
    test_results["pwd"] = run_comparison_test(
        "PWD Command", "pwd", expect_exact_stdout=True # PWD output should be exact
    )
    test_results["cmd_not_found"] = run_comparison_test(
        "Command Not Found", "a_very_unlikely_command_name_xyz", ignore_stderr=False # Error messages might differ slightly
    )
    test_results["exit_code_success"] = run_comparison_test(
        "Successful Command Exit Code", "echo success", expect_exact_stdout=True
    )
    test_results["exit_code_failure"] = run_comparison_test(
        "Failing Command Exit Code", "ls non_existent_file_abc", ignore_stderr=False # Check stderr for failure reason
    )

    # === Pipe Tests ===
    test_results["pipe_simple"] = run_comparison_test(
        "Simple Pipe", "echo 'one\\ntwo\\nthree' | grep two", expect_exact_stdout=True
    )
    test_results["pipe_multiple"] = run_comparison_test(
        "Multiple Pipes", "echo 'apple\\nbanana\\n apricot' | grep a | sort", expect_exact_stdout=True
    )
    test_results["pipe_stderr"] = run_comparison_test(
        "Pipe with Stderr", "ls non_existent_file_def | wc -l", expect_exact_stdout=True, ignore_stderr=False
    )


    # === Output Redirection Tests ===
    test_results["redir_out_overwrite"] = run_comparison_test(
        "Output Redirection (Overwrite >)",
        command="echo 'Hello > file' > out.txt",
        output_file_to_check="out.txt"
    )
    test_results["redir_out_append"] = run_comparison_test(
        "Output Redirection (Append >>)",
        # Run twice: once to create/overwrite, once to append
        command="echo 'Line 1' > append.txt && echo 'Line 2' >> append.txt",
        output_file_to_check="append.txt"
    )
    test_results["redir_out_stderr"] = run_comparison_test(
        "Stderr Redirection (2>)", # Assuming minishell supports 2> like bash
        command="ls non_existent_file_ghi 2> err.txt",
        output_file_to_check="err.txt",
        ignore_stderr=True # Main stderr might be empty if redirected
    )


     # === Input Redirection Tests ===
    test_results["redir_in_simple"] = run_comparison_test(
        "Input Redirection (<)",
        command="wc -l < input.txt",
        input_file_name="input.txt",
        input_file_content="line one\nline two\nline three",
        expect_exact_stdout=True
    )
    test_results["redir_in_pipe"] = run_comparison_test(
        "Input Redirection with Pipe",
        command="grep 'b' < data.txt | sort",
         input_file_name="data.txt",
        input_file_content="banana\napple\nblueberry\ncherry",
        expect_exact_stdout=True
    )


    # Add more complex tests: combinations, edge cases, builtins (cd, export etc.)

    # --- Test Summary ---
    print("\n" + "="*20 + " Test Summary " + "="*20)
    passed_count = 0
    failed_tests = []
    for name, result in test_results.items():
        status = "PASS" if result else "FAIL"
        print(f"{name:<30}: {status}")
        if result:
            passed_count += 1
        else:
            failed_tests.append(name)

    total_count = len(test_results)
    print("-" * (54))
    print(f"{passed_count} / {total_count} tests passed.")

    if failed_tests:
        print("\nFailed tests:")
        for test_name in failed_tests:
            print(f"- {test_name}")
        exit(1)
    else:
        print("\nAll tests passed!")
        exit(0)
