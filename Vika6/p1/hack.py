import subprocess

program_path = 'paging-policy.py'
all_policies = ['OPT']
# total_size = 652 # printed via unique_num_set
total_size = 268 # printed via unique_num_set

for policy in all_policies:
    print("\n" + policy)
    for cache_size in [652, 512]:
        additional_arguments = ['-f', '2.txt', '-C', str(cache_size), '-p', str(policy), '-c', '-N']
        subprocess.run(['python', program_path] + additional_arguments)