'''
Input: A text file containing the definition of a CFG written in BNF.
Output: A text file containing our C representation of the grammar.

Note: Multiple productions for a single non-terminal need to be specified on a
single line separated by the pipe operator.
e.g. <digit> : "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
'''

import sys

if len(sys.argv) != 2 and len(sys.argv) != 3:
    print("Usage: python bnf_converter.py path/to/grammar_file.txt <path/to/output_file.txt>", 
            file=sys.stderr)
    exit()

in_filepath = sys.argv[1]
if len(sys.argv) == 2:
    out_filepath = 'data/bnf_c.txt'
else:
    out_filepath = sys.argv[2]

# A list of dictionaries, where each dictionary represents one production
# dict = { "name": "", "rules": []}
grammar = []

# Open the grammar file and parse in each rule into a dictionary
f = open(in_filepath, "r")
for line in f:
    line_split = line.split("::=")
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
    output_file.write(f"\t{len(grammar)},\n")
    output_file.write("\t{\n")
    for i in range(len(grammar)):
        production = grammar[i]
        output_file.write("\t\t{\n")
        output_file.write(f'\t\t\t"{production["name"]}",\n')
        output_file.write(f'\t\t\t{len(production["rules"])},\n')
        output_file.write("\t\t\t{\n")

        for j in range(len(production['rules'])):
            rule = production['rules'][j]
            output_file.write("\t\t\t\t{")
            output_file.write(f"{len(rule)}, ")
            output_file.write(", ".join([f'"{element}"' for element in rule]))
            
            if j != len(production['rules']) - 1:
                output_file.write("},\n")
            else:
                output_file.write("}\n")

        output_file.write("\t\t\t}\n")

        if i != len(grammar) - 1:
            output_file.write("\t\t},\n")
        else:
            output_file.write("\t\t}\n")

    output_file.write("\t}\n")
    output_file.write("}\n")