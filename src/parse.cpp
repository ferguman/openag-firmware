//The parser will be implementing using the concept of a tree
//Operations that the parser uses.
//Create node(type, parent) -> types: branch_node, symbol_node, leaf_node
//Node.conncet(node)


//All expressions are a list - enforce this.

// Returns a tree of data items.
int parse(int token_list) {
/*
    If token= (
      Nl = new_node_list
      tree_node = add_nodes(rest_of_token_list, new list)
      If tree_node != nl Then
         Error encountered during parsing.
         Return 0
      End If
         Return nl
   Else
      Return 0
      Print error msg
   End
*/
   return 0;
}

/* 
Int add_nodes(token_list, nl)

   If no_more_tokens
      Return nl
   If next_token is ( Then
      new_nl = new list
      connect(nl, new_nl)
      Return add_nodes(remainder_of_token_list, new_nl)
   Else if next_token is ) Then
      If token is the last one then
         Return nl
      else
         Return add_nodes(remainder_of_token_list, nl.parent)
      End if
   //make symbol processing a sub-routine.
   Else if next_token is ‘ Then
      sym_node = new symbol node
      If next_token_is_list_start then
         sym_token_list = get_symbol_tokens
         remainder_of_token_list = get_list_after_sym_token_list
         sub_node = process_list_token_list(sym_token_list, sym_node)
         If sub_node = sym_node Then
            Return add_nodes(remainder_of_token_list, nl)
         Else
            Error processing symbol
            Return 0
         End if
      Else
         Connect(nl, sym_node)
         Connect(sym_node, current_token)
         Return add_nodes(remainder_of_token_list, nl)
      End If
   Else
      connect(nl, current_token)
      Return add_nodes(remainder_of_list, nl)

End If
*/

/*   Examples
#######
((1 2) (3 4))

n1
add_nodes[(1 2) (3 4)), n1]
N1->n2
add_nodes[1 2) (3 4)), n2]
N1-n2-1
add_nodes[2) (3 4)), n2]
N1-n2-1,2
add_nodes[) (3 4)), n2]
N1-n2-1,2
add_nodes[(3 4)), n1]
N1-n2-1,2
  -n3
add_nodes[3 4)), n3]
N1-n2-1,2
  -n3-3
add_nodes[4)), n3]
N1-n2-1,2
  -n3-3,4
add_nodes[)), n3]
Add_nodes[), n1] -> returns n1



####################
(Bar)
N1
add_nodes[bar), n1]
N1->bar
add_nodes[), n1] -> returns n1
*/
