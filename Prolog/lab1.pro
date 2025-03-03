predicates
	nondeterm person(symbol,integer)
	nondeterm parent(symbol,symbol)
	nondeterm happy(symbol)
	nondeterm sister(symbol,symbol)
	nondeterm brother(symbol,symbol)
	nondeterm hasAtLeastTwoChildren(symbol)
	nondeterm grandchild(symbol,symbol)
	nondeterm aunt(symbol,symbol)
clauses
	happy(Person):-
	parent(Person,X).
	
	sister(Person1,Person2):-
	parent(X,Person1) and
	parent(X,Person2) and
	person(Person1,2).
	
	brother(Person1,Person2):-
	parent(X,Person1) and
	parent(X,Person2) and
	person(Person1,1).
	
	hasAtLeastTwoChildren(Person):-
	parent(Person, Child) and
	sister(Child,X) or brother(Child,X).
	
	grandchild(GChild, G):-
	parent(G,X) and
	parent(X, GChild).
	
	aunt(A, Nephew):-
	sister(A,X) and
	parent(X,Nephew).
	
	person("VOVA", 1).
	person("MASHA", 2).
	person("VITYA", 1).
	person("TANYA",2).
	person("YULIA",2).
	parent("VOVA","VITYA").
	parent("MASHA", "VITYA").
	parent("TANYA", "VOVA").
	parent("TANYA", "MASHA").
	parent("TANYA", "YULIA").
goal
	happy(WhoHappy) and
	aunt(Who1,Who2).