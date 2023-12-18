'''
Input: A text file containing the definition of a CFG written in BNF.
Output: A text file containing our C representation of the grammar.

Note: Multiple productions for a single non-terminal need to be specified on a
single line separated by the pipe operator.
e.g. <digit> : "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
'''

import sys

if len(sys.argv) != 2:
    print("Usage: python bnf_converter.py path/to/grammar_file.txt", 
            file=sys.stderr)
    exit()

in_filepath = sys.argv[1]
out_filepath = "./bnf_c.txt"

# A list of dictionaries, where each dictionary represents one production
grammar = []

# Open the grammar file and parse in each rule into a dictionary
f = open(in_filepath, "r")
for line in f:
    line_split = line.split(":")
    rule_name = line_split[0].strip()
    rules = line_split[1].replace('"', '').strip().split(" | ")
    rules_formatted = []

    for rule in rules:
        if rule.find(" "):
            array = rule.split(" ")
            rules_formatted.append(array)
        else:
            rules_formatted.append(rule)

    production = {
        "name": rule_name,
        "rules": rules_formatted
    }
    grammar.append(production)

f.close()

##### PRINT IN C FORMAT #####

if len(production) == 0:
    with open(out_filepath, "w") as output_file:
            output_file.write("{}")
    exit()

with open(out_filepath, "w") as output_file:
    output_file.write("{\n")
    output_file.write("\t{\n")
    for production in grammar:
        output_file.write("\t\t{\n")
        output_file.write(f'\t\t\t"{production["name"]}",\n')
        output_file.write("\t\t\t{\n")
        for rule in production['rules']:
            output_file.write("\t\t\t\t{")
            output_file.write(", ".join([f'"{element}"' for element in rule]))
            output_file.write("}\n")
        output_file.write("\t\t\t}\n")
        output_file.write("\t\t}\n")
    output_file.write("\t}\n")
    output_file.write("}\n")