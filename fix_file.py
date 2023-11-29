import os
import re
import shutil

def restore_files(directory):
    pattern = re.compile(r'(.+?)\.Id_\d+\.(.+)$')

    for root, dirs, files in os.walk(directory):
        for filename in files:
            match = pattern.match(filename)
            if match:
                original_filename = f"{match.group(1)}.{match.group(2)}"
                original_filepath = os.path.join(root, original_filename)
                backup_filepath = os.path.join(root, filename)

                try:
                    # 尝试复制和删除操作
                    shutil.copyfile(backup_filepath, original_filepath)
                    os.remove(backup_filepath)
                    print(f"Restored {original_filepath} from {backup_filepath}")
                except Exception as e:
                    # 输出错误信息
                    print(f"Error processing {backup_filepath}: {e}")

# 获取当前工作目录
current_directory = os.getcwd()

# 调用函数
restore_files(current_directory)
