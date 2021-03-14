<h1><b>Function Names</b></h1>
<p>    Abbreviations must be intuitive</p>
<p>    Snake Case, but first Character of each part must be Uppercase. E.g. AST_Parse_Identifier()</p>
<h1><b>Variable Names</b></h1>
  <p>Constant Values must be all lower case, e.g. const double pi = 3.14</p>
  <p>Struct Member Names are as follows: <br> If it is one word, the first character must be uppercase, e.g. ASTNode* Node<br> If it is more than one word, abbreviations are all capitals and every new word starts with an Uppercase Character, e.g. int TNTThunderStruck</p>
  
<h1><b>Struct Names</b></h1>
  <p>See Variable Names</p>
  
<h1><b>Programming Practices</b></h1>
<h2><b>if Statements</b></h2>
If statements containing only one line of code should be like this:<br>

```
if (statement)
  code;
```

<h2><b>Memory Management</b></h2>
Ensure all memory allocated has functions to free it and that memory is always freed.<br>
Always confirm any memory being written to is not NULL. <br>
Pointers that are not in use (such as in struct Token), must always be set to NULL
