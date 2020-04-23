# Simplang Language v0.0.1-alpha

This Readme explains the motivations behind the language, development details, etc. For the language spec/standard, read the Spec.md file.



## On Pointers
I originally wanted to reinvent the pointer syntax as the following:
	`byte ptr x = &z; @x = 0x12;`
But I eventually decided against it because I realized how ingrained the c-style pointer syntax is w/i the programming community, that reinventing this wouldn't fly, regardless of its superiority.
