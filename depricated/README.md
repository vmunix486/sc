# sc

`sc` is an experimental language that I have thought up of that is very simple to make interpreters for, kinda like Brainf#ck but a billion times easier to use.

The main gist of the language is that every function uses a single character, so as an example, printing "Hello, World!" can be achieved by doing
```
p("Hello, World!\n");
```

The syntax of the language itself is similar to C/C++/Javascript, in how there are semicolons (;) after every statement. (sort of, newlines and semicolons are parsed the exact same way)

This was mostly created during downtime on a trip to Toronto I had, and it was written in such a way that you do not need to make a lexer for it, since that would be too inefficient.

# How it works under the hood

So for inquiring minds, how my interpreter works is that it goes line by line, scanning each character line by line, and if the character corresponds to a function, then it will do stuff.

As an example, I'll break down how it parses the plain Hello, World!, shown above.

```
p("Hello, World!\n");
```

First off, it starts out on the first line, then it goes to the first character. The first character is a `p`.

```
-> p("Hello, World!\n");
   ^
   |
   Found 'p'
```

The letter `p` corresponds to a print statement, so it changes `funcs.print` from `0` to `1`.

After that, it finds an opening parenthesis.

```
-> p("Hello, World!\n");
    ^
    |
    Found `(`
```

Just like before, opening parenthesis turns `funcs.inparenthesis` from a `0` to a `1`.

And you can probably tell where it goes from here...

```
-> p("Hello, World!\n");
     ^
     |
     Found '"', turning on funcs.inquotes...

-> p("Hello, World!\n");
      ^
      |
      Adding string to buffer...
```
**And so on...**
```
-> p("Hello, World!\n");
                   ^
                   |
                   Found '\', turning on funcs.escapechar...

-> p("Hello, World!\n");
                    ^
                    |
                    Found 'n' while funcs.escapechar is on, adding newline to string...

-> p("Hello, World!\n");
                     ^
                     |
                     Found '"' while funcs.inquotes is on. Turning off funcs.inquotes, and printing out string...

-> p("Hello, World!\n");
                      ^
                      |
                      Found ')' while funcs.inparenthesis is on. Turning off funcs.inparenthesis...

-> p("Hello, World!\n");
                       ^
                       |
                       Found ';'. Doing nothing...

```

And there we have it. That's basically how the `sci` interpreter works under the hood, and it's pretty simple code-wise as well. Just lots of variables and `if` statments.
