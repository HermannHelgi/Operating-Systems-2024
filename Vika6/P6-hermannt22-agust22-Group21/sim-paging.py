#!/usr/bin/env python3

#
# Put your solution into the three functions in this file
#
import matplotlib.pyplot as plt 


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
                pfn = int(address_in_hex[0:-3],20)
                page_access_list.append(pfn)
                if line[0] == 'I':
                    instruction_page_set.add(pfn)

    return page_access_list, instruction_page_set


def plot_memory_access(page_access_list, png_file=None, instruction_page_set=None):
    number_of_bins = len(page_access_list) // 1000
    normalized_dictionary = {}
    count = 0
    for elem in sorted(page_access_list):
        if (elem not in normalized_dictionary):
            normalized_dictionary[elem] = count
            count += 1

    array2d = [[0 for i in range(number_of_bins)] for k in range(len(normalized_dictionary))]

    for index, page in enumerate(page_access_list):
        array2d[normalized_dictionary[page]][((index//1000)-1)] = 1

    plt.imshow(array2d, cmap='binary', aspect='auto')

    plt.gca().invert_yaxis()
    plt.xlabel('Bins / Time of Access')
    plt.ylabel('Page Numbers')
    plt.title('Memory Access Patterns')

    if png_file:
        plt.savefig(png_file)
    else:
        plt.show()
    return


def export_page_trace(page_access_list, output_file):
    new_list = []
    new_list.append(page_access_list[0])
    for i in range(0, len(page_access_list)-1):
        elem_1 = page_access_list[i]
        if elem_1 != new_list[-1]:
            new_list.append(elem_1)

    with open(output_file,"w") as new_output_file:
        for i in new_list:
            new_output_file.write(str(i)+ "\n")

    return



if __name__ == "__main__":
    filename = "trace-stydemo.txt"
    page_list, page_set = get_page_list(filename)
    plot_memory_access(page_list)