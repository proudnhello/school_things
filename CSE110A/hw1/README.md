# hw1

## Part 1

#### Describe how you implemented the new required tokens.

Implementing SEMI was fairly simple. I added a new single character check, much the same as ADD or MULT, except it checked if peek_char resulted in a ';'. If it did, I returned a Lexeme who's token was Token.SEMI, and who's id was ";".

For the update of ID, I changed the while loop to be "while self.ss.peek_char() in CHARS or **self.ss.peek_char() in NUMS:**". Because this is already inside the if statement that starts a new ID, which remains unchanged, I know the first charcter must be a letter. As such, this finds a letter to start an ID, but then the ID is allowed to continue if following characters are numbers.

For the change of NUMS, I added "or self.ss.peek_char() == "."" to both the inital if statement and the internal while loop. This allows decimals to start with and contain '.'. I also created a variable that would count the number of decimal points. If the program ever reads in two decimal points, it breaks out of the while loop. This is because while a number can't contain two decimal points, it can start with one, so 12.34.56 could be tokenized as two numbers, 12.34 and .56. This allows the next time token is clalled, it will find that first '.' and start a new number. Finally, as a token cannot end with a '.', at the end of the while loop for NUMS, I check to see if the last character in value is a '.', and throw an exception if it is.

Finally, to implement INCR, I added a new token type called INCR. I then added on to the check for ADD. Immediately after checking to see if the first character is a '+', I then check to see if the following character is also a '+'. If it is, I return a INCR, "++" lexmeme, otherwise I return an ADD, "+" lememe as normal.

#### Write down your timings for running the provided test cases. Comment on their scaling.

10:    0.0004086494445800781s  
100:   0.003059864044189453s  
1000:  0.041379451751708984s  
10000: 1.1401715278625488s  

The scaling for these test cases starts out actually rather favorable. 10 to 100, and 100 to 1000 show an almost linear increace, with 10x the lexemes taking slightly more or slightly less than 10x the time, which while not great, isn't awful. However, what could be dismissed as slight variation with 10 to 100 increacing by slightly less than 10x the time, and 100 to 1000 taking slighly more than 10x the time, reveals itself to be a growth rate worse than linear with the jump rate between 1000 and 10000. 10000 lexemes takes roughtly 28x the time to scan than 1000, which clearly shows a worse than linear groth rate. As such, with very large files, this scanner will very quickly take a very long time to process everything. I am, however, not entirely sure where that extra time comes from. Linear makes sense, as the scanner simply reads in every character, one at a time, so it taking longer being directly related to the number of characters checks out. Perhaps the lexemes are also getting larger with the 10000 lexeme test, maybe there's another time sink I don't quite understand, or perhaps my computer simply started doing something in the background, because I'm not sure where exactly this jump comes from.

## Part 2

#### Describe your RE definitions.

Most of the RE definitions were fairly simple, with all of them except for ID, NUM, and HNUM being pretty much as simple as adding a RE that contains an exact match, ie for INCR my RE was just "++". For ID, my RE looked like \[a-zA-Z\]\[0-9a-zA-Z\]\*.The first part can be any lower or uppercase letter, which is what an ID must start with, while the second, optional part, is any digit 0-9, or any upper or lowercase number repeated 0 or more times, which allows the ID to contain any number of letters or numbers after the first. NUM, on the other hand, looks like (\[0-9\]+)|(\[0-9\]*\.\[0-9\]+). On the left side, we simply have any numerical digit repeated 1 or more times, which makes up our standard non-decimal numbers. On the right side, however, we have any numerical digit repeated 0 or more times, followed by a decimal point, and then any numberical digit repeated one or more times. This accounts for all decimal numbers. Finally, hexadecimals are defined by ((0x)|(0X))\[0-9a-fA-F\]+. All this says is 0x or 0X, followed by any numberal digit or letter a-f one or more times.

#### Write down your timings for running the EM Scanner on the provided test cases. You do not need to run it past size 100. Comment on the performance scaling and how it relates to Part 1. Explain why the speed is faster or slower than part 1.

10:    0.03810310363769531s  
100:   3.456085681915283s  

Compared to part 1, this scaling is atrocious. 100 lexemes took almost 100 times longer than 10, which means the scalling was 10 times worse than part 1, even ignoring the absolute time, and it could very well get worse with 1000 lexemes. This does, however, make sense. Whereas the Naive scanner only had to read in every character than check it against a few if statements, our EM scanner has to remove a character, then check to see the string is a match for any RE, and continue doing so until it matches a statement. This is bad enough, and checking the string against the REs is already probably slower than the naive scanner, but once we find a match, we then have to do the same thing for the entire string again, only removing the single match. This is horribly slow, especially with larger files, so I would predict that the jump from 100 to 1000 is probably even worse than than jump from 10 to 100.

## Part 3

#### Describe how your SOS Scanner is different from the EM Scanner.

The SOS scanner uses a built in function of our RE matcher that automatically matches the first occurence of one of our tokens in the input string. This means that we can simply skip the removing one character at a time from the input string until one of the tokens match, and simply find the first match and return it. This, while much more efficent, does lead to the potential of more than one RE matching the input string. To solve this, we have to be sure to tokenize the input as the largest match, which does require us to iterate over every match made by the RE. This will slow it down a little bit, but it should still be much faster than th EM scanner

#### Report your timings for running the test inputs and compare against the timings for the EM Scanner (at least for 10 and 100).

10:    0.0008141994476318359s  
100:   0.004354238510131836s  
1000:  0.042269229888916016s  
10000: 0.5365636348724365s  

These are way, *way* faster than the EM Scanner, and while the shorter tests are slower than the Naive scanner, the growth is slower, so by the 10000 lexeme test, it actually runs faster than it.

#### Describe why there is a performance difference between the SOS and EM Scanner.

The EM Scanner uses the fullmatch function, which only finds a match if an entire string matches the RE. To complensate for this fact, we repeatedly remove the last character in a file until we find a match. This is horribly inefficent. Imagine we have a file that a million characters, each of which is its own token. To find the first token, we need remove and then run fullmatch one million times. After that, to find the next token, whatever it may be, we're going to have to do the same process again. On the other hand, the SOS Scanner uses the less precise match function, which instead looks at the front of the string, and finds whatever matches it can there. While this does mean we need to make sure that we tokenize the longest match for the SOS scanner, asides from that it is massively faster. Imagine we have the same million character and token long test file. We only need to run match a total of a million time to find all of the matches, whereas the EM scanner had to run fullmatch a million times just to find the first token. While match is probably less efficent than fullmatch, having to run it so many fewer times easily makes up for that fact.

## Part 4

#### Describe how your NG Scanner is different from the SOS Scanner.

Where the SOS scanner would try and match the start of the string with every possible token RE, the NG scanner instead builds up a massive RE, and then only runs match on it once. This does mean we have to consider how our tokens interact with eachother, which is annoying, but with us only having to run match the one time, it should be faster.

#### Report your timings for running the test inputs and compare against the timings for the SOS Scanner.

10:    0.0005736351013183594s  
100:   0.0023419857025146484s  
1000:  0.02003169059753418s  
10000: 0.32833003997802734s  

As expected, the NG Scanner was faster than the SOS scanner, with it being roughly twice as fast as the SOS scanner.

#### Explain the performance difference between the SOS and NG Scanner.

When finding a match, the SOS scanner has to iterate over every single token, and try to match the start of the string to that token's RE. By contrast, the NG Scanner simply matches the start of the string once to the combined RE. While the NG scanner loses some time at the begining building it's RE, that is more than made up by only having to run match once every time we find a new token. Each individual match is probably slower, due to the longer RE, and perhaps if our loop in our SOS scanner were more efficient, the timings might be more comperable. But even then, with how the match function is set up, the NG scanner gets to perform short circut evaluation, where if the first option in our RE matches, it stops checking. On the other hand, the SOS scanner needs to check every single token to find the longest match. So even if running match on every token's RE was just as fast as running match on a combination of every token's RE, the NG scanner would still be faster due to that short circut evaluation.

#### If you shuffle the tokens list in tokens.py, is the NG Scanner guaranteed to be correct? Why or why not?

Absolutely not. Due to all of the token identification happening in a single RE, match will find the first match in our large RE. This means that if our tokens have any overlap, if a token could be a prefix for another or if a token could be valid in two different ways, messing up the order could have unintended consiquences. In our case, having the token for add before the one for increment would cause "++" to be identified as two add tokens, which is obviously incorrect. Similarly, placing hex numbers below both IDs and numbers would probably result in hex numbers being identified as a number and an ID, ie 0x1234 would become the number 0, and the ID x1234.
