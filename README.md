# Sap
Sap is a compiled programming language written in C. It is partially inspired by ADA. Currently it is still in development (Very limited functionality), an example of what factorial might look like when finished:

```
use "std/io";

func factorial(var number : int) -> int
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
