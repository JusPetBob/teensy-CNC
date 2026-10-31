Import("env")

print(env.Dump())

import os
if os.name == 'nt':
    env.Execute('$PROJECT_DIR/helper/pre_compile.bat')
else:
    env.Execute('$PROJECT_DIR/helper/pre_compile.sh')