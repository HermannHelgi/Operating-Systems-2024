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

    # TODO: Implement (remove this comment when you implemented something)
    
    return page_access_list, instruction_page_set


def plot_memory_access(page_access_list, png_file=None, instruction_page_set=None):

    number_of_bins = len(page_access_list) // 1000
    normalized_dictionary = {}
    count = 0
    for elem in sorted(page_access_list):
        if (elem not in normalized_dictionary):
            normalized_dictionary[elem] = count
            count += 1

    array2d = [[0 for i in range(len(normalized_dictionary))] for k in range(number_of_bins)]

    for bin, page in enumerate(page_access_list):
        array2d[bin//1000][normalized_dictionary[page]] = 1

    plt.imshow(array2d, cmap='gray', aspect='auto')

    # Add labels and title for better interpretation
    plt.xlabel('Page Numbers')
    plt.ylabel('Bins / Time of Access')
    plt.title('Memory Access Patterns')

    # Show or save the plot
    if png_file:
        plt.savefig(png_file)
    else:
        plt.show()

    return


def export_page_trace(page_access_list, output_file):

    # TODO: Implement (remove this comment before submission if you implemented somthing)

    return
