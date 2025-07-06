import os
from runfiles import Runfiles # Import from the bazel-runfiles library

def get_local_path(path):
    """
    Looks up the actual path of a data file using Bazel's runfiles system.

    Args:
        relative_path_in_workspace (str): The path to the file relative
                                           to your Bazel workspace root.
                                           e.g., "_main/logo_automation/my_data_file.txt"
    Returns:
        str: The absolute path to the file in the runfiles environment.
    """
    # todo, optimize
    r = Runfiles.Create()
    actual_path = r.Rlocation(path)
    return str(actual_path).replace('\\', '/')