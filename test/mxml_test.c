#include "mxml.h"

mxml_type_t	type_cb(mxml_node_t *node);
int		whitespace_cb(mxml_node_t *node, int where);

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line args */
     char *argv[])			/* I - Command-line args */
{
  FILE			*fp;		/* File to read */
  mxml_node_t		*tree,		/* XML tree */
			*node;		/* Node which should be in test.xml */
  static const char	*types[] =	/* Strings for node types */
			{
			  "MXML_ELEMENT",
			  "MXML_INTEGER",
			  "MXML_OPAQUE",
			  "MXML_REAL",
			  "MXML_TEXT"
			};


  if (argc != 2)
  {
    fputs("Usage: testmxml filename.xml\n", stderr);
    return (1);
  }


  tree = mxmlNewElement(MXML_NO_PARENT, "element");

  if (!tree)
  {
    fputs("ERROR: No parent node in basic test!\n", stderr);
    return (1);
  }

  if (tree->type != MXML_ELEMENT)
  {
    fprintf(stderr, "ERROR: Parent has type %s (%d), expected MXML_ELEMENT!\n",
            tree->type < MXML_ELEMENT || tree->type > MXML_TEXT ?
	        "UNKNOWN" : types[tree->type], tree->type);
    mxmlDelete(tree);
    return (1);
  }

  if (strcmp(tree->value.element.name, "element"))
  {
    fprintf(stderr, "ERROR: Parent value is \"%s\", expected \"element\"!\n",
            tree->value.element.name);
    mxmlDelete(tree);
    return (1);
  }

  mxmlNewInteger(tree, 123);
  mxmlNewOpaque(tree, "opaque");
  mxmlNewReal(tree, 123.4f);
  mxmlNewText(tree, 1, "text");

  node = tree->child;

  if (!node)
  {
    fputs("ERROR: No first child node in basic test!\n", stderr);
    mxmlDelete(tree);
    return (1);
  }

  if (node->type != MXML_INTEGER)
  {
    fprintf(stderr, "ERROR: First child has type %s (%d), expected MXML_INTEGER!\n",
            node->type < MXML_ELEMENT || node->type > MXML_TEXT ?
	        "UNKNOWN" : types[node->type], node->type);
    mxmlDelete(tree);
    return (1);
  }

  if (node->value.integer != 123)
  {
    fprintf(stderr, "ERROR: First child value is %d, expected 123!\n",
            node->value.integer);
    mxmlDelete(tree);
    return (1);
  }

  node = node->next;

  if (!node)
  {
    fputs("ERROR: No second child node in basic test!\n", stderr);
    mxmlDelete(tree);
    return (1);
  }

  if (node->type != MXML_OPAQUE)
  {
    fprintf(stderr, "ERROR: Second child has type %s (%d), expected MXML_OPAQUE!\n",
            node->type < MXML_ELEMENT || node->type > MXML_TEXT ?
	        "UNKNOWN" : types[node->type], node->type);
    mxmlDelete(tree);
    return (1);
  }

  if (!node->value.opaque || strcmp(node->value.opaque, "opaque"))
  {
    fprintf(stderr, "ERROR: Second child value is \"%s\", expected \"opaque\"!\n",
            node->value.opaque ? node->value.opaque : "(null)");
    mxmlDelete(tree);
    return (1);
  }

  node = node->next;

  if (!node)
  {
    fputs("ERROR: No third child node in basic test!\n", stderr);
    mxmlDelete(tree);
    return (1);
  }

  if (node->type != MXML_REAL)
  {
    fprintf(stderr, "ERROR: Third child has type %s (%d), expected MXML_REAL!\n",
            node->type < MXML_ELEMENT || node->type > MXML_TEXT ?
	        "UNKNOWN" : types[node->type], node->type);
    mxmlDelete(tree);
    return (1);
  }

  if (node->value.real != 123.4f)
  {
    fprintf(stderr, "ERROR: Third child value is %f, expected 123.4!\n",
            node->value.real);
    mxmlDelete(tree);
    return (1);
  }

  node = node->next;

  if (!node)
  {
    fputs("ERROR: No fourth child node in basic test!\n", stderr);
    mxmlDelete(tree);
    return (1);
  }

  if (node->type != MXML_TEXT)
  {
    fprintf(stderr, "ERROR: Fourth child has type %s (%d), expected MXML_TEXT!\n",
            node->type < MXML_ELEMENT || node->type > MXML_TEXT ?
	        "UNKNOWN" : types[node->type], node->type);
    mxmlDelete(tree);
    return (1);
  }

  if (!node->value.text.whitespace ||
      !node->value.text.string || strcmp(node->value.text.string, "text"))
  {
    fprintf(stderr, "ERROR: Fourth child value is %d,\"%s\", expected 1,\"text\"!\n",
            node->value.text.whitespace,
	    node->value.text.string ? node->value.text.string : "(null)");
    mxmlDelete(tree);
    return (1);
  }

  mxmlDelete(tree->child);
  mxmlDelete(tree->child);
  mxmlDelete(tree->child);
  mxmlDelete(tree->child);

  if (tree->child)
  {
    fputs("ERROR: Child pointer not NULL after deleting all children!\n", stderr);
    return (1);
  }

  if (tree->last_child)
  {
    fputs("ERROR: Last child pointer not NULL after deleting all children!\n", stderr);
    return (1);
  }

  mxmlDelete(tree);



  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    perror(argv[1]);
    return (1);
  }

 /*
  * Read the file...
  */

  tree = mxmlLoadFile(NULL, fp, type_cb);

  fclose(fp);

  if (!tree)
  {
    fputs("Unable to read XML file!\n", stderr);
    return (1);
  }

  if (!strcmp(argv[1], "test.xml"))
  {

    if ((node = mxmlFindElement(tree, tree, "choice", NULL, NULL,
                                MXML_DESCEND)) == NULL)
    {
      fputs("Unable to find first <choice> element in XML tree!\n", stderr);
      mxmlDelete(tree);
      return (1);
    }

    if ((node = mxmlFindElement(node, tree, "choice", NULL, NULL,
                                MXML_NO_DESCEND)) == NULL)
    {
      fputs("Unable to find second <choice> element in XML tree!\n", stderr);
      mxmlDelete(tree);
      return (1);
    }
  }


  mxmlSaveFile(tree, stdout, whitespace_cb);

  mxmlDelete(tree);

  return (0);
}


/*
 * 'type_cb()' - XML data type callback for mxmlLoadFile()...
 */

mxml_type_t				/* O - Data type */
type_cb(mxml_node_t *node)		/* I - Element node */
{
  const char	*type;			/* Type string */


  if ((type = mxmlElementGetAttr(node, "type")) == NULL)
    type = node->value.element.name;

  if (!strcmp(type, "integer"))
    return (MXML_INTEGER);
  else if (!strcmp(type, "opaque") || !strcmp(type, "pre"))
    return (MXML_OPAQUE);
  else if (!strcmp(type, "real"))
    return (MXML_REAL);
  else
    return (MXML_TEXT);
}


int					/* O - Whitespace char or 0 */
whitespace_cb(mxml_node_t *node,	/* I - Element node */
              int         where)	/* I - Open or close tag? */
{
  const char *name;			/* Name of element */

  name = node->value.element.name;

  if (!strcmp(name, "html") || !strcmp(name, "head") || !strcmp(name, "body") ||
      !strcmp(name, "pre") || !strcmp(name, "p") ||
      !strcmp(name, "h1") || !strcmp(name, "h2") || !strcmp(name, "h3") ||
      !strcmp(name, "h4") || !strcmp(name, "h5") || !strcmp(name, "h6"))
  {
   /*
    * Newlines before open and after close...
    */

    if (where == MXML_WS_BEFORE_OPEN || where == MXML_WS_AFTER_CLOSE)
      return ('\n');
  }
  else if (!strcmp(name, "dl") || !strcmp(name, "ol") || !strcmp(name, "ul"))
  {
   /*
    * Put a newline before and after list elements...
    */

    return ('\n');
  }
  else if (!strcmp(name, "dd") || !strcmp(name, "dt") || !strcmp(name, "li"))
  {
   /*
    * Put a tab before <li>'s, <dd>'s, and <dt>'s, and a newline after them...
    */

    if (where == MXML_WS_BEFORE_OPEN)
      return ('\t');
    else if (where == MXML_WS_AFTER_CLOSE)
      return ('\n');
  }

  return (0);
}
