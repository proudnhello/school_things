import re

# perform the local value numbering optimization
def LVN(program):

    # returns 3 items:

    # Convert the program to basic blocks
    blocks = blockizer(program)

    number_blocks = [[]]
    new_vars = []
    counter = 0
    last_numbered = dict()
    computed_values = dict()
    first_patch = []
    converted_lines = 0

    for block in blocks:
        # clean the block, patch, and computed values
        computed_values = dict()
        new_block = []
        first_patch = []
        # clean the dict b/c otherwise things will get overlaping numbers
        # this *probably* would be fine but it scares me
        last_numbered.clear()
        for line in block:
            # Due to the way my regex things work, some lines will match more than one statement
            # This variable prevents this from happening
            # I would use an elif statement, but b/c i want to use the matches I make, this doesn't work properly
            matched = False
            line = line.replace(" ", "")
            # If no matches are found (say, for a label), this will cause the new line to be added at the end unchanged
            new_line = line

            # First, provide numbers for two argument functions
            # No IO/const checking needs to be done, b/c they cannot appear here
            matches = re.match("(\S+)=(\S+)\((\S+),(\S+)\)", line)
            if matches != None:
                matched = True
                # Convert the match object into more easily understood values
                destination = matches[1]
                old_destination = last_numbered.get(destination)
                instruction = matches[2]
                arg1 = matches[3]
                new_arg1 = last_numbered.get(arg1)
                arg2 = matches[4]
                new_arg2 = last_numbered.get(arg2)

                # Start by checking to see if the arguments have been numbered already
                if new_arg1 == None:
                    # If they haven't been, create a numbered variant, and it to the list of new vars, the first patch, and the last_numbered dict
                    # Then increment counter
                    new_arg1 = arg1 + "_" + str(counter)
                    last_numbered[arg1] = new_arg1
                    first_patch += [new_arg1 + "=" + arg1 + ";"]
                    new_vars += [new_arg1]
                    counter += 1
                # do the same for arg2
                if new_arg2 == None:
                    new_arg2 = arg2 + "_" + str(counter)
                    last_numbered[arg2] = new_arg2
                    first_patch += [new_arg2 + "=" + arg2 + ";"]
                    new_vars += [new_arg2]
                    counter += 1

                # Then, update the destination
                new_destination = destination + "_" + str(counter)
                last_numbered[destination] = new_destination
                counter += 1
                new_vars += [new_destination]

                # if the destination has never appeared before, add it to the front patch
                # (probably not nessisary, here for safety)
                if old_destination == None:
                    first_patch += [new_destination + "=" + destination + ";"]
                
                # Create the key that we will use for our local value numbering
                computation = instruction + "(" + new_arg1 + "," + new_arg2 + ");"
                resulting_vr = computed_values.get(computation)

                # check to see if there's a vr that currently has that value
                if resulting_vr != None:
                    # If there is, just make the next line destination = found vr
                    new_line = new_destination + "=" + resulting_vr +";"
                    converted_lines += 1
                else:
                    # otherwise, put the proper, actual computation into the new_line
                    new_line = new_destination + "=" + instruction + "(" + new_arg1 + "," + new_arg2 + ");"

                # Add the computation and destination to the dict
                computed_values[computation] = new_destination
                # if we're looking at the friendly comunative values, add the reverse as well
                if instruction in ["addi", "multi", "eqi"]:
                    computed_values[(instruction + "(" + new_arg2 + "," + new_arg1 + ");")] = new_destination

            # Next, we match single operator functions 
            matches = re.match("(\S+)=(\S+)\((\S+)\)", line)
            if matched == False and matches != None:
                # Convert the match object into more easily understood values
                destination = matches[1]
                old_destination = last_numbered.get(destination)
                instruction = matches[2]
                arg1 = matches[3]
                new_arg1 = last_numbered.get(arg1)
                new_destination = None

                # Start by checking to see if the argument or destination is a IO variable or a constant, and therefore should not be numbered
                if re.match("_new_name_.*|vr.*", arg1) == None:
                    new_arg1 = arg1
                if re.match("_new_name_.*|vr.*", destination) == None:
                    new_destination = destination
                else:
                    new_destination = destination + "_" + str(counter)
                    last_numbered[destination] = new_destination
                    counter += 1
                    new_vars += [new_destination]

                # If arg1 is a vr/normal variable, check to see if it has been already used
                if new_arg1 == None:
                    # If they haven't been, create a numbered variant, and it to the list of new vars, the first patch, and the last_numbered dict
                    # Then increment counter
                    new_arg1 = arg1 + "_" + str(counter)
                    last_numbered[arg1] = new_arg1
                    first_patch += [new_arg1 + "=" + arg1 + ";"]
                    counter += 1
                    new_vars += [new_arg1]

                # if the destination has never appeared before and is not an IO var, add it to the front patch
                # (probably not nessisary, here for safety)
                if old_destination == None and new_destination != destination:
                    first_patch += [new_destination + "=" + destination + ";"]

                # Finally, update the new line
                new_line = new_destination + "=" + instruction + "(" + new_arg1 + ");"

            # Finally, we match the funny third case that I have created via my own hubris due to me using my own parser
            # This occurs when there is no need to convert type in an assignment statement because I was lazy in hw4
            # It therefore only sets a normal var to a vr
            # For this reason, there will be no lvn optimizations applied, b/c why would you bother?
            matches = re.match("(\S+)=\((\S+)\)", line)
            if matched == False and matches != None:
                 # Convert the match object into more easily understood values
                destination = matches[1]
                old_destination = last_numbered.get(destination)
                arg1 = matches[2]
                new_arg1 = last_numbered.get(arg1)
                new_destination = None

                # Start by checking to see if the arguments have been numbered already
                if new_arg1 == None:
                    # If they haven't been, create a numbered variant, and it to the list of new vars, the first patch, and the last_numbered dict
                    # Then increment counter
                    new_arg1 = arg1 + "_" + str(counter)
                    last_numbered[arg1] = new_arg1
                    first_patch += [new_arg1 + "=" + arg1 + ";"]
                    counter += 1
                    new_vars += [new_arg1]

                # Then, update the destination
                new_destination = destination + "_" + str(counter)
                last_numbered[destination] = new_destination
                counter += 1
                new_vars += [new_destination]

                # if the destination has never appeared before, add it to the front patch
                # (probably not nessisary, here for safety)
                if old_destination == None:
                    first_patch += [new_destination + "=" + destination + ";"]
                
                # Finally, update the new line
                new_line = new_destination + "=" + "(" + new_arg1 + ");"

            # add the new line to the new block
            new_block += [new_line]
        # install some patches
        # start by determining if the first line has any control flow, saving and removing it if it does
        control_flow = ""
        if re.match(".*:|beq\(.*\);|bneq\(.*\);|branch\(.*\);", new_block[0]) != None:
            control_flow = new_block.pop(0)
        back_patch = []
        for var, num_var in last_numbered.items():
            back_patch += [(var + "=" + num_var + ";")]
        new_block = [control_flow] + first_patch + new_block + back_patch        
        # add to new block to the list of blocks
        number_blocks.append(new_block)
                
    
    # 1. a new program (list of classier instructions)
    # with the LVN optimization applied

    # 2. a list of new variables required (e.g. numbered virtual
    # registers and program variables)

    # 3. a number with how many instructions were replaced 

    # End by converting the block back into a single list
    end_program = []
    for block in number_blocks:
        end_program += block

    return end_program, new_vars, converted_lines

# This should take the program and break it up into a list of basic blocks
# Due to us needing to stitch the program together, every block except the first will contain some control flow in index 0
def blockizer(program):
    blocks = [[]]
    current_block = 0

    for line in program:
        # I feel like i remember hearing that .* is vile and to be avoided, but uh, too bad i guess
        # If we have some control flow, move to the next block, and append the contol flow to it
        if re.match(".*:|beq\(.*\);|bneq\(.*\);|branch\(.*\);", line) != None:
            current_block += 1
            blocks.append([line])
        else:
        # Otherwise, just add the line to the current block
            blocks[current_block] += [line]

    return blocks

# This will return a numberized version of the block, the new vrs created, 
# the patch that should be placed in the front, and the patch that should go in the back
# def numberizer(block):
#     num_blk = []
#     nvar = []
#     fpatch = []
#     bpatch = []
#     last_var = dict()
#     for line in block:
#         line = line.replace(" ", "")
#         matches = re.match("(\S+)=(\S+)\((\S+),(\S+)\)", line)
#         if matches
