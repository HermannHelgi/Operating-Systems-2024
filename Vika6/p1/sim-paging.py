#!/usr/bin/env python3

#
# Put your solution into the three functions in this file
#

def get_page_list(filename):
    # Expected functionality: Read content of file (valgrind/lackey output), and then
    # - find all lines containing memory access:
    #   Line has I, L, M, S at the beginning (first two columns), then a space
    #   After that an address in hex notation
    #   Finally, a comma and an access size in byte
    # - construct an ordered list of memory pages accessed (based on the address)
    # - construct an set of memory pages that contain instructions (based on address in 'I' lines)
    page_access_list = []
    instruction_page_set = set()

    with open(filename,'r') as file:

        for line in file:
                line = line.strip()
                line = line.split()
                if (line[0] == 'I' or line[0] == 'S' or line[0] == 'L' or line[0] == 'M'):
                    line[1] = line[1].split(',')
                    address_in_hex = line[1][0]
                    pfn = int(address_in_hex[0:5],20)
                    page_access_list.append(pfn)
                    if line[0] == 'I':
                        instruction_page_set.add(pfn)
    return page_access_list, instruction_page_set


def plot_memory_access(page_access_list, png_file=None, instruction_page_set=None):

    # TODO: Implement (remove this comment before submission if you implemented somthing)

    return


def export_page_trace(page_access_list, output_file):

    for i in range(0, len(page_access_list)-1):
        elem_1 = page_access_list[i]
        elem_2 = page_access_list[i+1]

        if(elem_1 == elem_2):
            print("Penis:" + elem_1)
    

    return

#print(get_page_list("testinput.txt"))

page_list = get_page_list("testinput.txt")

print(page_list)