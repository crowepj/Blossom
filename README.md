# WattLang
WattLang is a compiled programming language written in C. It is partially inspired by ADA. Currently it is still in development (Developing Parser), an example of what factorial might look like when finished:

```
use "std/io";

func factorial(var number : int) 
{
  if (number > 0) 
  {
    ret factorial(number - 1) * number;
  }
  else
  {
    ret 1;
  }
}

func main() 
{
  var ex := factorial(4);
  io::print(ex);
}

```

Contributors are appreciated, please read contributing.md first
