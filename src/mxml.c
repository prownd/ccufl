/*
 * "$Id: mxml.c 34 2003-06-14 23:56:47Z mike $"
 * Attribute support code for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*
 * Include necessary headers...
 */

#include "mxml.h"


/*
 * 'mxmlElementGetAttr()' - Get an attribute.
 *
 * This function returns NULL if the node is not an element or the
 * named attribute does not exist.
 */

const char *				/* O - Attribute value or NULL */
mxmlElementGetAttr(mxml_node_t *node,	/* I - Element node */
                   const char  *name)	/* I - Name of attribute */
{
  int	i;				/* Looping var */
  mxml_attr_t	*attr;			/* Cirrent attribute */


 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name)
    return (NULL);

 /*
  * Look for the attribute...
  */

  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
       i > 0;
       i --, attr ++)
    if (!strcmp(attr->name, name))
      return (attr->value);

 /*
  * Didn't find attribute, so return NULL...
  */

  return (NULL);
}


/*
 * 'mxmlElementSetAttr()' - Set an attribute.
 *
 * If the named attribute already exists, the value of the attribute
 * is replaced by the new string value. The string value is copied
 * into the element node. This function does nothing if the node is
 * not an element.
 */

void
mxmlElementSetAttr(mxml_node_t *node,	/* I - Element node */
                   const char  *name,	/* I - Name of attribute */
                   const char  *value)	/* I - Attribute value */
{
  int		i;			/* Looping var */
  mxml_attr_t	*attr;			/* New attribute */


 /*
  * Range check input...
  */

  if (!node || node->type != MXML_ELEMENT || !name || !value)
    return;

 /*
  * Look for the attribute...
  */

  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
       i > 0;
       i --, attr ++)
    if (!strcmp(attr->name, name))
    {
     /*
      * Replace the attribute value and return...
      */

      free(attr->value);

      attr->value = strdup(value);

      return;
    }

 /*
  * Attribute not found, so add a new one...
  */

  if (node->value.element.num_attrs == 0)
    attr = malloc(sizeof(mxml_attr_t));
  else
    attr = realloc(node->value.element.attrs,
                   (node->value.element.num_attrs + 1) * sizeof(mxml_attr_t));

  if (!attr)
  {
    fprintf(stderr, "Unable to allocate memory for attribute '%s' in element %s!\n",
            name, node->value.element.name);
    return;
  }

  node->value.element.attrs = attr;
  attr += node->value.element.num_attrs;

  attr->name  = strdup(name);
  attr->value = strdup(value);

  if (!attr->name || !attr->value)
  {
    if (attr->name)
      free(attr->name);

    if (attr->value)
      free(attr->value);

    fprintf(stderr, "Unable to allocate memory for attribute '%s' in element %s!\n",
            name, node->value.element.name);

    return;
  }
    
  node->value.element.num_attrs ++;
}



/*
 * Include necessary headers...
 */

#include "mxml.h"


/*
 * Local functions...
 */

static int	mxml_parse_element(mxml_node_t *node, FILE *fp);
static int	mxml_write_node(mxml_node_t *node, FILE *fp,
		                int (*cb)(mxml_node_t *, int), int col);
static int	mxml_write_string(const char *s, FILE *fp);
static int	mxml_write_ws(mxml_node_t *node, FILE *fp, 
                              int (*cb)(mxml_node_t *, int), int ws, int col);


/*
 * 'mxmlLoadFile()' - Load a file into an XML node tree.
 *
 * The nodes in the specified file are added to the specified top node.
 * If no top node is provided, the XML file MUST be well-formed with a
 * single parent node like <?xml> for the entire file. The callback
 * function returns the value type that should be used for child nodes.
 * If MXML_NO_CALLBACK is specified then all child nodes will be either
 * MXML_ELEMENT or MXML_TEXT nodes.
 */

mxml_node_t *				/* O - First node or NULL if the file could not be read. */
mxmlLoadFile(mxml_node_t *top,		/* I - Top node */
             FILE        *fp,		/* I - File to read from */
             mxml_type_t (*cb)(mxml_node_t *))
					/* I - Callback function or MXML_NO_CALLBACK */
{
  mxml_node_t	*node,			/* Current node */
		*parent;		/* Current parent node */
  int		ch,			/* Character from file */
		whitespace;		/* Non-zero if whitespace seen */
  char		buffer[16384],		/* String buffer */
		*bufptr;		/* Pointer into buffer */
  mxml_type_t	type;			/* Current node type */


 /*
  * Read elements and other nodes from the file...
  */

  bufptr     = buffer;
  parent     = top;
  whitespace = 0;

  if (cb && parent)
    type = (*cb)(parent);
  else
    type = MXML_TEXT;

  while ((ch = getc(fp)) != EOF)
  {
    if ((ch == '<' || (isspace(ch) && type != MXML_OPAQUE)) && bufptr > buffer)
    {
     /*
      * Add a new value node...
      */

      *bufptr = '\0';

      switch (type)
      {
	case MXML_INTEGER :
            node = mxmlNewInteger(parent, strtol(buffer, &bufptr, 0));
	    break;

	case MXML_OPAQUE :
            node = mxmlNewOpaque(parent, buffer);
	    break;

	case MXML_REAL :
            node = mxmlNewReal(parent, strtod(buffer, &bufptr));
	    break;

	case MXML_TEXT :
            node = mxmlNewText(parent, whitespace, buffer);
	    break;

        default : /* Should never happen... */
	    node = NULL;
	    break;
      }	  

      if (*bufptr)
      {
       /*
        * Bad integer/real number value...
	*/

        fprintf(stderr, "Bad %s value '%s' in parent <%s>!\n",
	        type == MXML_INTEGER ? "integer" : "real", buffer,
		parent ? parent->value.element.name : "null");
	break;
      }

      bufptr     = buffer;
      whitespace = isspace(ch) && type == MXML_TEXT;

      if (!node)
      {
       /*
	* Just print error for now...
	*/

	fprintf(stderr, "Unable to add value node of type %d to parent <%s>!\n",
	        type, parent ? parent->value.element.name : "null");
	break;
      }
    }
    else if (isspace(ch) && type == MXML_TEXT)
      whitespace = 1;

   /*
    * Add lone whitespace node if we are starting a new element and have
    * existing whitespace...
    */

    if (ch == '<' && whitespace && type == MXML_TEXT)
    {
     /*
      * Peek at the next character and only do this if we are starting
      * an open tag...
      */

      ch = getc(fp);
      ungetc(ch, fp);

      if (ch != '/')
      {
	mxmlNewText(parent, whitespace, "");
	whitespace = 0;
      }

      ch = '<';
    }

    if (ch == '<')
    {
     /*
      * Start of open/close tag...
      */

      bufptr = buffer;

      while ((ch = getc(fp)) != EOF)
        if (isspace(ch) || ch == '>' || (ch == '/' && bufptr > buffer))
	  break;
	else if (bufptr < (buffer + sizeof(buffer) - 1))
	{
	  *bufptr++ = ch;

	  if ((bufptr - buffer) == 3 && !strncmp(buffer, "!--", 3))
	    break;
        }

      *bufptr = '\0';

      if (!strcmp(buffer, "!--"))
      {
       /*
        * Gather rest of comment...
	*/

	while ((ch = getc(fp)) != EOF)
	{
	  if (ch == '>' && bufptr > (buffer + 4) &&
	      !strncmp(bufptr - 2, "--", 2))
	    break;
	  else if (bufptr < (buffer + sizeof(buffer) - 1))
            *bufptr++ = ch;
	  else
	  {
            fprintf(stderr, "Comment too long in file under parent <%s>!\n",
	            parent ? parent->value.element.name : "null");
	    break;
	  }
	}

       /*
        * Error out if we didn't get the whole comment...
	*/

        if (ch != '>')
	  break;

       /*
        * Otherwise add this as an element under the current parent...
	*/

	*bufptr = '\0';

	if (!mxmlNewElement(parent, buffer))
	{
	 /*
	  * Just print error for now...
	  */

	  fprintf(stderr, "Unable to add comment node to parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
	  break;
	}
      }
      else if (buffer[0] == '!')
      {
       /*
        * Gather rest of declaration...
	*/

	do
	{
	  if (ch == '>')
	    break;
	  else if (bufptr < (buffer + sizeof(buffer) - 1))
            *bufptr++ = ch;
	  else
	  {
            fprintf(stderr, "Declaration too long in file under parent <%s>!\n",
	            parent ? parent->value.element.name : "null");
	    break;
	  }
	}
        while ((ch = getc(fp)) != EOF);

       /*
        * Error out if we didn't get the whole declaration...
	*/

        if (ch != '>')
	  break;

       /*
        * Otherwise add this as an element under the current parent...
	*/

	*bufptr = '\0';

	node = mxmlNewElement(parent, buffer);
	if (!node)
	{
	 /*
	  * Just print error for now...
	  */

	  fprintf(stderr, "Unable to add declaration node to parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
	  break;
	}

       /*
	* Descend into this node, setting the value type as needed...
	*/

	parent = node;

	if (cb && parent)
	  type = (*cb)(parent);
      }
      else if (buffer[0] == '/')
      {
       /*
        * Handle close tag...
	*/

        if (!parent || strcmp(buffer + 1, parent->value.element.name))
	{
	 /*
	  * Close tag doesn't match tree; print an error for now...
	  */

	  fprintf(stderr, "Mismatched close tag <%s> under parent <%s>!\n",
	          buffer, parent->value.element.name);
          break;
	}

       /*
        * Keep reading until we see >...
	*/

        while (ch != '>' && ch != EOF)
	  ch = getc(fp);

       /*
	* Ascend into the parent and set the value type as needed...
	*/

	parent = parent->parent;

	if (cb && parent)
	  type = (*cb)(parent);
      }
      else
      {
       /*
        * Handle open tag...
	*/

        node = mxmlNewElement(parent, buffer);

	if (!node)
	{
	 /*
	  * Just print error for now...
	  */

	  fprintf(stderr, "Unable to add element node to parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
	  break;
	}

        if (isspace(ch))
          ch = mxml_parse_element(node, fp);
        else if (ch == '/')
	{
	  if ((ch = getc(fp)) != '>')
	  {
	    fprintf(stderr, "Expected > but got '%c' instead for element <%s/>!\n",
	            ch, buffer);
            break;
	  }

	  ch = '/';
	}

	if (ch == EOF)
	  break;

        if (ch != '/')
	{
	 /*
	  * Descend into this node, setting the value type as needed...
	  */

	  parent = node;

	  if (cb && parent)
	    type = (*cb)(parent);
	}
      }

      bufptr  = buffer;
    }
    else if (ch == '&')
    {
     /*
      * Add character entity to current buffer...  Currently we only
      * support &lt;, &amp;, &gt;, &nbsp;, &quot;, &#nnn;, and &#xXXXX;...
      */

      char	entity[64],		/* Entity string */
		*entptr;		/* Pointer into entity */


      entity[0] = ch;
      entptr    = entity + 1;

      while ((ch = getc(fp)) != EOF)
        if (!isalnum(ch) && ch != '#')
	  break;
	else if (entptr < (entity + sizeof(entity) - 1))
	  *entptr++ = ch;
	else
	{
	  fprintf(stderr, "Entity name too long under parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
          break;
	}

      *entptr = '\0';

      if (ch != ';')
      {
	fprintf(stderr, "Entity name \"%s\" not terminated under parent <%s>!\n",
	        entity, parent ? parent->value.element.name : "null");
        break;
      }

      if (entity[1] == '#')
      {
	if (entity[2] == 'x')
	  ch = strtol(entity + 3, NULL, 16);
	else
	  ch = strtol(entity + 2, NULL, 10);
      }
      else if (!strcmp(entity, "&amp"))
        ch = '&';
      else if (!strcmp(entity, "&gt"))
        ch = '>';
      else if (!strcmp(entity, "&lt"))
        ch = '<';
      else if (!strcmp(entity, "&nbsp"))
        ch = 0xa0;
      else if (!strcmp(entity, "&quot"))
        ch = '\"';
      else
      {
	fprintf(stderr, "Entity name \"%s;\" not supported under parent <%s>!\n",
	        entity, parent ? parent->value.element.name : "null");
        break;
      }

      if (ch < 128)
      {
       /*
        * Plain ASCII doesn't need special encoding...
	*/

	if (bufptr < (buffer + sizeof(buffer) - 1))
          *bufptr++ = ch;
	else
	{
          fprintf(stderr, "String too long in file under parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
	  break;
	}
      }
      else
      {
       /*
        * Use UTF-8 encoding for the Unicode char...
	*/

	if (bufptr < (buffer + sizeof(buffer) - 5))
	{
	  if (ch < 2048)
	  {
	    *bufptr++ = 0xc0 | (ch >> 6);
	    *bufptr++ = 0x80 | (ch & 63);
          }
	  else if (ch < 65536)
	  {
	    *bufptr++ = 0xe0 | (ch >> 12);
	    *bufptr++ = 0x80 | ((ch >> 6) & 63);
	    *bufptr++ = 0x80 | (ch & 63);
	  }
	  else
	  {
	    *bufptr++ = 0xf0 | (ch >> 18);
	    *bufptr++ = 0x80 | ((ch >> 12) & 63);
	    *bufptr++ = 0x80 | ((ch >> 6) & 63);
	    *bufptr++ = 0x80 | (ch & 63);
	  }
	}
	else
	{
          fprintf(stderr, "String too long in file under parent <%s>!\n",
	          parent ? parent->value.element.name : "null");
	  break;
	}
      }
    }
    else if (type == MXML_OPAQUE || !isspace(ch))
    {
     /*
      * Add character to current buffer...
      */

      if (bufptr < (buffer + sizeof(buffer) - 1))
        *bufptr++ = ch;
      else
      {
        fprintf(stderr, "String too long in file under parent <%s>!\n",
	        parent ? parent->value.element.name : "null");
	break;
      }
    }
  }

 /*
  * Find the top element and return it...
  */

  if (parent)
  {
    while (parent->parent != top)
      parent = parent->parent;
  }

  return (parent);
}


/*
 * 'mxmlSaveFile()' - Save an XML tree to a file.
 *
 * The callback argument specifies a function that returns a whitespace
 * character or nul (0) before and after each element. If MXML_NO_CALLBACK
 * is specified, whitespace will only be added before MXML_TEXT nodes
 * with leading whitespace and before attribute names inside opening
 * element tags.
 */

int					/* O - 0 on success, -1 on error. */
mxmlSaveFile(mxml_node_t *node,		/* I - Node to write */
             FILE        *fp,		/* I - File to write to */
	     int         (*cb)(mxml_node_t *, int))
					/* I - Whitespace callback or MXML_NO_CALLBACK */
{
  int	col;				/* Final column */


 /*
  * Write the node...
  */

  if ((col = mxml_write_node(node, fp, cb, 0)) < 0)
    return (-1);

  if (col > 0)
    if (putc('\n', fp) < 0)
      return (-1);

 /*
  * Return 0 (success)...
  */

  return (0);
}


/*
 * 'mxml_parse_element()' - Parse an element for any attributes...
 */

static int				/* O - Terminating character */
mxml_parse_element(mxml_node_t *node,	/* I - Element node */
                   FILE        *fp)	/* I - File to read from */
{
  int	ch,				/* Current character in file */
	quote;				/* Quoting character */
  char	name[256],			/* Attribute name */
	value[256],			/* Attribute value */
	*ptr;				/* Pointer into name/value */


 /*
  * Loop until we hit a >, /, ?, or EOF...
  */

  while ((ch = getc(fp)) != EOF)
  {
   /*
    * Skip leading whitespace...
    */

    if (isspace(ch))
      continue;

   /*
    * Stop at /, ?, or >...
    */

    if (ch == '/' || ch == '?')
    {
     /*
      * Grab the > character and print an error if it isn't there...
      */

      quote = getc(fp);

      if (quote != '>')
      {
        fprintf(stderr, "Expected '>' after '%c' for element %s, but got '%c'!\n",
	        ch, node->value.element.name, quote);
        ch = EOF;
      }

      break;
    }
    else if (ch == '>')
      break;

   /*
    * Read the attribute name...
    */

    name[0] = ch;
    ptr     = name + 1;

    while ((ch = getc(fp)) != EOF)
      if (isspace(ch) || ch == '=' || ch == '/' || ch == '>' || ch == '?')
        break;
      else if (ptr < (name + sizeof(name) - 1))
        *ptr++ = ch;
      else
      {
        fprintf(stderr, "Attribute name too long for element %s!\n",
	        node->value.element.name);
        return (EOF);
      }

    *ptr = '\0';

    if (ch == '=')
    {
     /*
      * Read the attribute value...
      */

      if ((ch = getc(fp)) == EOF)
      {
        fprintf(stderr, "Missing value for attribute '%s' in element %s!\n",
	        name, node->value.element.name);
        return (EOF);
      }

      if (ch == '\'' || ch == '\"')
      {
       /*
        * Read quoted value...
	*/

        quote = ch;
	ptr   = value;

        while ((ch = getc(fp)) != EOF)
	  if (ch == quote)
	    break;
	  else if (ptr < (value + sizeof(value) - 1))
            *ptr++ = ch;
	  else
	  {
            fprintf(stderr, "Attribute value too long for attribute '%s' in element %s!\n",
	            name, node->value.element.name);
            return (EOF);
	  }

        *ptr = '\0';
      }
      else
      {
       /*
        * Read unquoted value...
	*/

	value[0] = ch;
	ptr      = value + 1;

	while ((ch = getc(fp)) != EOF)
	  if (isspace(ch) || ch == '=' || ch == '/' || ch == '>')
            break;
	  else if (ptr < (value + sizeof(value) - 1))
            *ptr++ = ch;
	  else
	  {
            fprintf(stderr, "Attribute value too long for attribute '%s' in element %s!\n",
	            name, node->value.element.name);
            return (EOF);
	  }

        *ptr = '\0';
      }
    }
    else
      value[0] = '\0';

   /*
    * Save last character in case we need it...
    */

    if (ch == '/' || ch == '>' || ch == '?')
      ungetc(ch, fp);

   /*
    * Set the attribute...
    */

    mxmlElementSetAttr(node, name, value);
  }

  return (ch);
}


/*
 * 'mxml_write_node()' - Save an XML node to a file.
 */

static int				/* O - Column or -1 on error */
mxml_write_node(mxml_node_t *node,	/* I - Node to write */
                FILE        *fp,	/* I - File to write to */
	        int         (*cb)(mxml_node_t *, int),
					/* I - Whitespace callback */
		int         col)	/* I - Current column */
{
  int		i;			/* Looping var */
  int		n;			/* Chars written */
  mxml_attr_t	*attr;			/* Current attribute */


  while (node != NULL)
  {
   /*
    * Print the node value...
    */

    switch (node->type)
    {
      case MXML_ELEMENT :
          col = mxml_write_ws(node, fp, cb, MXML_WS_BEFORE_OPEN, col);

          if ((n = fprintf(fp, "<%s", node->value.element.name)) < 0)
	    return (-1);

          col += n;

	  for (i = node->value.element.num_attrs, attr = node->value.element.attrs;
	       i > 0;
	       i --, attr ++)
	  {
	    if ((col + strlen(attr->name) + strlen(attr->value) + 3) > MXML_WRAP)
	    {
	      if (putc('\n', fp) < 0)
	        return (-1);

	      col = 0;
	    }
	    else
	    {
	      if (putc(' ', fp) < 0)
	        return (-1);

	      col ++;
	    }

	    if ((n = fprintf(fp, "%s=\"%s\"", attr->name, attr->value)) < 0)
              return (-1);

            col += n;
	  }

	  if (node->child)
	  {
           /*
	    * The ? and ! elements are special-cases and have no end tags...
	    */

	    if (node->value.element.name[0] == '?')
	    {
	      if (fputs("?>\n", fp) < 0)
	        return (-1);

              col = 0;
            }
	    else if (putc('>', fp) < 0)
	      return (-1);
	    else
	      col ++;

            col = mxml_write_ws(node, fp, cb, MXML_WS_AFTER_OPEN, col);

	    if ((col = mxml_write_node(node->child, fp, cb, col)) < 0)
	      return (-1);

            if (node->value.element.name[0] != '?' &&
	        node->value.element.name[0] != '!')
	    {
              col = mxml_write_ws(node, fp, cb, MXML_WS_BEFORE_CLOSE, col);

	      if ((n = fprintf(fp, "</%s>", node->value.element.name)) < 0)
	        return (-1);

              col += n;

              col = mxml_write_ws(node, fp, cb, MXML_WS_AFTER_CLOSE, col);
	    }
	  }
	  else if (node->value.element.name[0] == '!')
	  {
	    if (putc('>', fp) < 0)
	      return (-1);
	    else
	      col ++;

            col = mxml_write_ws(node, fp, cb, MXML_WS_AFTER_OPEN, col);
          }
	  else if (fputs("/>", fp) < 0)
	    return (-1);
	  else
	  {
	    col += 2;

            col = mxml_write_ws(node, fp, cb, MXML_WS_AFTER_OPEN, col);
	  }
          break;

      case MXML_INTEGER :
	  if (node->prev)
	  {
	    if (col > MXML_WRAP)
	    {
	      if (putc('\n', fp) < 0)
	        return (-1);

	      col = 0;
	    }
	    else if (putc(' ', fp) < 0)
	      return (-1);
	    else
	      col ++;
          }

          if ((n = fprintf(fp, "%d", node->value.integer)) < 0)
	    return (-1);

	  col += n;
          break;

      case MXML_OPAQUE :
          if (mxml_write_string(node->value.opaque, fp) < 0)
	    return (-1);

          col += strlen(node->value.opaque);
          break;

      case MXML_REAL :
	  if (node->prev)
	  {
	    if (col > MXML_WRAP)
	    {
	      if (putc('\n', fp) < 0)
	        return (-1);

	      col = 0;
	    }
	    else if (putc(' ', fp) < 0)
	      return (-1);
	    else
	      col ++;
          }

          if ((n = fprintf(fp, "%f", node->value.real)) < 0)
	    return (-1);

	  col += n;
          break;

      case MXML_TEXT :
	  if (node->value.text.whitespace && col > 0)
	  {
	    if (col > MXML_WRAP)
	    {
	      if (putc('\n', fp) < 0)
	        return (-1);

	      col = 0;
	    }
	    else if (putc(' ', fp) < 0)
	      return (-1);
	    else
	      col ++;
          }

          if (mxml_write_string(node->value.text.string, fp) < 0)
	    return (-1);

	  col += strlen(node->value.text.string);
          break;
    }

   /*
    * Next node...
    */

    node = node->next;
  }

  return (col);
}


/*
 * 'mxml_write_string()' - Write a string, escaping & and < as needed.
 */

static int				/* O - 0 on success, -1 on failure */
mxml_write_string(const char *s,	/* I - String to write */
                  FILE       *fp)	/* I - File to write to */
{
  while (*s)
  {
    if (*s == '&')
    {
      if (fputs("&amp;", fp) < 0)
        return (-1);
    }
    else if (*s == '<')
    {
      if (fputs("&lt;", fp) < 0)
        return (-1);
    }
    else if (*s == '>')
    {
      if (fputs("&gt;", fp) < 0)
        return (-1);
    }
    else if (*s == '\"')
    {
      if (fputs("&quot;", fp) < 0)
        return (-1);
    }
    else if (*s & 128)
    {
     /*
      * Convert UTF-8 to Unicode constant...
      */

      int	ch;			/* Unicode character */


      ch = *s & 255;

      if ((ch & 0xe0) == 0xc0)
      {
        ch = ((ch & 0x1f) << 6) | (s[1] & 0x3f);
	s ++;
      }
      else if ((ch & 0xf0) == 0xe0)
      {
        ch = ((((ch * 0x0f) << 6) | (s[1] & 0x3f)) << 6) | (s[2] & 0x3f);
	s += 2;
      }

      if (ch == 0xa0)
      {
       /*
        * Handle non-breaking space as-is...
	*/

        if (fputs("&nbsp;", fp) < 0)
	  return (-1);
      }
      else if (fprintf(fp, "&#x%x;", ch) < 0)
	return (-1);
    }
    else if (putc(*s, fp) < 0)
      return (-1);

    s ++;
  }

  return (0);
}


/*
 * 'mxml_write_ws()' - Do whitespace callback...
 */

static int				/* O - New column */
mxml_write_ws(mxml_node_t *node,	/* I - Current node */
              FILE        *fp,		/* I - File to write to */
              int         (*cb)(mxml_node_t *, int),
					/* I - Callback function */
	      int         ws,		/* I - Where value */
	      int         col)		/* I - Current column */
{
  int	ch;				/* Whitespace character */


  if (cb && (ch = (*cb)(node, ws)) != 0)
  {
    if (putc(ch, fp) < 0)
      return (-1);
    else if (ch == '\n')
      col = 0;
    else if (ch == '\t')
    {
      col += MXML_TAB;
      col = col - (col % MXML_TAB);
    }
    else
      col ++;
  }

  return (col);
}


/*
 * Include necessary headers...
 */

#include "mxml.h"


/*
 * Local functions...
 */

static mxml_node_t	*mxml_new(mxml_node_t *parent, mxml_type_t type);


/*
 * 'mxmlAdd()' - Add a node to a tree.
 *
 * Adds the specified node to the parent. If the child argument is not
 * NULL, puts the new node before or after the specified child depending
 * on the value of the where argument. If the child argument is NULL,
 * puts the new node at the beginning of the child list (MXML_ADD_BEFORE)
 * or at the end of the child list (MXML_ADD_AFTER). The constant
 * MXML_ADD_TO_PARENT can be used to specify a NULL child pointer.
 */

void
mxmlAdd(mxml_node_t *parent,		/* I - Parent node */
        int         where,		/* I - Where to add, MXML_ADD_BEFORE or MXML_ADD_AFTER */
        mxml_node_t *child,		/* I - Child node for where or MXML_ADD_TO_PARENT */
	mxml_node_t *node)		/* I - Node to add */
{
/*  fprintf(stderr, "mxmlAdd(parent=%p, where=%d, child=%p, node=%p)\n", parent,
         where, child, node);*/

 /*
  * Range check input...
  */

  if (!parent || !node)
    return;

 /*
  * Remove the node from any existing parent...
  */

  if (node->parent)
    mxmlRemove(node);

 /*
  * Reset pointers...
  */

  node->parent = parent;

  switch (where)
  {
    case MXML_ADD_BEFORE :
        if (!child || child == parent->child || child->parent != parent)
	{
	 /*
	  * Insert as first node under parent...
	  */

	  node->next = parent->child;

	  if (parent->child)
	    parent->child->prev = node;
	  else
	    parent->last_child = node;

	  parent->child = node;
	}
	else
	{
	 /*
	  * Insert node before this child...
	  */

	  node->next = child;
	  node->prev = child->prev;

	  if (child->prev)
	    child->prev->next = node;
	  else
	    parent->child = node;

	  child->prev = node;
	}
        break;

    case MXML_ADD_AFTER :
        if (!child || child == parent->last_child || child->parent != parent)
	{
	 /*
	  * Insert as last node under parent...
	  */

	  node->parent = parent;
	  node->prev   = parent->last_child;

	  if (parent->last_child)
	    parent->last_child->next = node;
	  else
	    parent->child = node;

	  parent->last_child = node;
        }
	else
	{
	 /*
	  * Insert node after this child...
	  */

	  node->prev = child;
	  node->next = child->next;

	  if (child->next)
	    child->next->prev = node;
	  else
	    parent->last_child = node;

	  child->next = node;
	}
        break;
  }
}


/*
 * 'mxmlDelete()' - Delete a node and all of its children.
 *
 * If the specified node has a parent, this function first removes the
 * node from its parent using the mxmlRemove() function.
 */

void
mxmlDelete(mxml_node_t *node)		/* I - Node to delete */
{
  int	i;				/* Looping var */


/*  fprintf(stderr, "mxmlDelete(node=%p)\n", node);*/

 /*
  * Range check input...
  */

  if (!node)
    return;

 /*
  * Remove the node from its parent, if any...
  */

  mxmlRemove(node);

 /*
  * Delete children...
  */

  while (node->child)
    mxmlDelete(node->child);

 /*
  * Now delete any node data...
  */

  switch (node->type)
  {
    case MXML_ELEMENT :
        if (node->value.element.name)
	  free(node->value.element.name);

	if (node->value.element.num_attrs)
	{
	  for (i = 0; i < node->value.element.num_attrs; i ++)
	  {
	    if (node->value.element.attrs[i].name)
	      free(node->value.element.attrs[i].name);
	    if (node->value.element.attrs[i].value)
	      free(node->value.element.attrs[i].value);
	  }

          free(node->value.element.attrs);
	}
        break;
    case MXML_INTEGER :
       /* Nothing to do */
        break;
    case MXML_OPAQUE :
        if (node->value.opaque)
	  free(node->value.opaque);
        break;
    case MXML_REAL :
       /* Nothing to do */
        break;
    case MXML_TEXT :
        if (node->value.text.string)
	  free(node->value.text.string);
        break;
  }

 /*
  * Free this node...
  */

  free(node);
}


/*
 * 'mxmlNewElement()' - Create a new element node.
 *
 * The new element node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * element node has no parent.
 */

mxml_node_t *				/* O - New node */
mxmlNewElement(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
               const char  *name)	/* I - Name of element */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Range check input...
  */

  if (!name)
    return (NULL);

 /*
  * Create the node and set the element name...
  */

  if ((node = mxml_new(parent, MXML_ELEMENT)) != NULL)
    node->value.element.name = strdup(name);

  return (node);
}


/*
 * 'mxmlNewInteger()' - Create a new integer node.
 *
 * The new integer node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * integer node has no parent.
 */

mxml_node_t *				/* O - New node */
mxmlNewInteger(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
               int         integer)	/* I - Integer value */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Range check input...
  */

  if (!parent)
    return (NULL);

 /*
  * Create the node and set the element name...
  */

  if ((node = mxml_new(parent, MXML_INTEGER)) != NULL)
    node->value.integer = integer;

  return (node);
}


/*
 * 'mxmlNewOpaque()' - Create a new opaque string.
 *
 * The new opaque node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * opaque node has no parent. The opaque string must be nul-terminated and
 * is copied into the new node.
 */

mxml_node_t *				/* O - New node */
mxmlNewOpaque(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
              const char  *opaque)	/* I - Opaque string */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Range check input...
  */

  if (!parent || !opaque)
    return (NULL);

 /*
  * Create the node and set the element name...
  */

  if ((node = mxml_new(parent, MXML_OPAQUE)) != NULL)
    node->value.opaque = strdup(opaque);

  return (node);
}


/*
 * 'mxmlNewReal()' - Create a new real number node.
 *
 * The new real number node is added to the end of the specified parent's
 * child list. The constant MXML_NO_PARENT can be used to specify that
 * the new real number node has no parent.
 */

mxml_node_t *				/* O - New node */
mxmlNewReal(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
            double      real)		/* I - Real number value */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Range check input...
  */

  if (!parent)
    return (NULL);

 /*
  * Create the node and set the element name...
  */

  if ((node = mxml_new(parent, MXML_REAL)) != NULL)
    node->value.real = real;

  return (node);
}


/*
 * 'mxmlNewText()' - Create a new text fragment node.
 *
 * The new text node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * text node has no parent. The whitespace parameter is used to specify
 * whether leading whitespace is present before the node. The text
 * string must be nul-terminated and is copied into the new node.  
 */

mxml_node_t *				/* O - New node */
mxmlNewText(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
            int         whitespace,	/* I - 1 = leading whitespace, 0 = no whitespace */
	    const char  *string)	/* I - String */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Range check input...
  */

  if (!parent || !string)
    return (NULL);

 /*
  * Create the node and set the text value...
  */

  if ((node = mxml_new(parent, MXML_TEXT)) != NULL)
  {
    node->value.text.whitespace = whitespace;
    node->value.text.string     = strdup(string);
  }

  return (node);
}


/*
 * 'mxmlRemove()' - Remove a node from its parent.
 *
 * Does not free memory used by the node - use mxmlDelete() for that.
 * This function does nothing if the node has no parent.
 */

void
mxmlRemove(mxml_node_t *node)		/* I - Node to remove */
{
 /*
  * Range check input...
  */

/*  fprintf(stderr, "mxmlRemove(node=%p)\n", node);*/

  if (!node || !node->parent)
    return;

 /*
  * Remove from parent...
  */

  if (node->prev)
    node->prev->next = node->next;
  else
    node->parent->child = node->next;

  if (node->next)
    node->next->prev = node->prev;
  else
    node->parent->last_child = node->prev;

  node->parent = NULL;
  node->prev   = NULL;
  node->next   = NULL;
}


/*
 * 'mxml_new()' - Create a new node.
 */

static mxml_node_t *			/* O - New node */
mxml_new(mxml_node_t *parent,		/* I - Parent node */
         mxml_type_t type)		/* I - Node type */
{
  mxml_node_t	*node;			/* New node */


 /*
  * Allocate memory for the node...
  */

  if ((node = calloc(1, sizeof(mxml_node_t))) == NULL)
    return (NULL);

 /*
  * Set the node type...
  */

  node->type = type;

 /*
  * Add to the parent if present...
  */

  if (parent)
    mxmlAdd(parent, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, node);

 /*
  * Return the new node...
  */

  return (node);
}


/*
 * End of "$Id: mxml-node.c 34 2003-06-14 23:56:47Z mike $".
 */
/*
 * "$Id: mxml-search.c 34 2003-06-14 23:56:47Z mike $"
 *
 * Search/navigation functions for mini-XML, a small XML-like file
 * parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlFindElement() - Find the named element.
 *   mxmlWalkNext()    - Walk to the next logical node in the tree.
 *   mxmlWalkPrev()    - Walk to the previous logical node in the tree.
 */

/*
 * Include necessary headers...
 */

#include "mxml.h"


/*
 * 'mxmlFindElement()' - Find the named element.
 *
 * The search is constrained by the name, attribute name, and value; any
 * NULL names or values are treated as wildcards, so different kinds of
 * searches can be implemented by looking for all elements of a given name
 * or all elements with a specific attribute. The descend argument determines
 * whether the search descends into child nodes; normally you will use
 * MXML_DESCEND_FIRST for the initial search and MXML_NO_DESCEND to find
 * additional direct descendents of the node. The top node argument
 * constrains the search to a particular node's children.
 */

mxml_node_t *				/* O - Element node or NULL */
mxmlFindElement(mxml_node_t *node,	/* I - Current node */
                mxml_node_t *top,	/* I - Top node */
                const char  *name,	/* I - Element name or NULL for any */
		const char  *attr,	/* I - Attribute name, or NULL for none */
		const char  *value,	/* I - Attribute value, or NULL for any */
		int         descend)	/* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
  const char	*temp;			/* Current attribute value */


 /*
  * Range check input...
  */

  if (!node || !top || (!attr && value))
    return (NULL);

 /*
  * Start with the next node...
  */

  node = mxmlWalkNext(node, top, descend);

 /*
  * Loop until we find a matching element...
  */

  while (node != NULL)
  {
   /*
    * See if this node matches...
    */

    if (node->type == MXML_ELEMENT &&
        node->value.element.name &&
	(!name || !strcmp(node->value.element.name, name)))
    {
     /*
      * See if we need to check for an attribute...
      */

      if (!attr)
        return (node);			/* No attribute search, return it... */

     /*
      * Check for the attribute...
      */

      if ((temp = mxmlElementGetAttr(node, attr)) != NULL)
      {
       /*
        * OK, we have the attribute, does it match?
	*/

	if (!value || !strcmp(value, temp))
	  return (node);		/* Yes, return it... */
      }
    }

   /*
    * No match, move on to the next node...
    */

    if (descend == MXML_DESCEND)
      node = mxmlWalkNext(node, top, MXML_DESCEND);
    else
      node = node->next;
  }

  return (NULL);
}


/*
 * 'mxmlWalkNext()' - Walk to the next logical node in the tree.
 *
 * The descend argument controls whether the first child is considered
 * to be the next node. The top node argument constrains the walk to
 * the node's children.
 */

mxml_node_t *				/* O - Next node or NULL */
mxmlWalkNext(mxml_node_t *node,		/* I - Current node */
             mxml_node_t *top,		/* I - Top node */
             int         descend)	/* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
  if (!node)
    return (NULL);
  else if (node->child && descend)
    return (node->child);
  else if (node->next)
    return (node->next);
  else if (node->parent && node->parent != top)
  {
    node = node->parent;

    while (!node->next)
      if (node->parent == top || !node->parent)
        return (NULL);
      else
        node = node->parent;

    return (node->next);
  }
  else
    return (NULL);
}


/*
 * 'mxmlWalkPrev()' - Walk to the previous logical node in the tree.
 *
 * The descend argument controls whether the previous node's last child
 * is considered to be the previous node. The top node argument constrains
 * the walk to the node's children.
 */

mxml_node_t *				/* O - Previous node or NULL */
mxmlWalkPrev(mxml_node_t *node,		/* I - Current node */
             mxml_node_t *top,		/* I - Top node */
             int         descend)	/* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
  if (!node)
    return (NULL);
  else if (node->prev)
  {
    if (node->prev->last_child && descend)
    {
     /*
      * Find the last child under the previous node...
      */

      node = node->prev->last_child;

      while (node->last_child)
        node = node->last_child;

      return (node);
    }
    else
      return (node->prev);
  }
  else if (node->parent != top)
    return (node->parent);
  else
    return (NULL);
}

