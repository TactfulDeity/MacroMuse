import subprocess

command = "git ls-files -z --full-name -- '*.cpp' '*.hpp' | xargs -0 -r clang-format -i --style=file"

subprocess.run(command, shell=True, check=True)