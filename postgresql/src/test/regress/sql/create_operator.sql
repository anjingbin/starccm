--
-- CREATE_OPERATOR
--

CREATE OPERATOR ## ( 
   leftarg = path,
   rightarg = path,
   procedure = path_inter,
   commutator = ## 
);

CREATE OPERATOR <% (
   leftarg = point,
   rightarg = widget,
   procedure = pt_in_widget,
   commutator = >% ,
   negator = >=% 
);

CREATE OPERATOR @#@ (
   rightarg = int4,		-- left unary 
   procedure = int4fac 
);

CREATE OPERATOR #@# (
   leftarg = int4,		-- right unary
   procedure = int4fac 
);

CREATE OPERATOR #%# ( 
   leftarg = int4,		-- right unary 
   procedure = int4fac 
);

