[+ AutoGen5 template tmp ddf hin +]
[+ CASE (suffix) +]
[+ ==  tmp +][+ (sprintf "%s%s AutoGen5 template %s%s" "[" "+" "+" "]") +]
[+ FOR category +]
category = {
	cat_name = [+ cat_name +];
[+ IF (exist?"cat_descript") +]	cat_descript = "[+ (get "cat_descript") +]";[+ ENDIF +]
	};
[+ ENDFOR category +]
[+ FOR class +]
	class = {
	class_name = [+ class_name +];
};
[+ ENDFOR class +]
[+ FOR category +][+ (sprintf "%s%s%s " "[" "+" " FOR ")  +][+ cat_name +][+ (sprintf " %s%s" "+" "]")  +]
parameter = {

	cat_name = [+ cat_name +];

	para_name = [+ (sprintf "%s%s " "[" "+")  +](get "para_name")[+ (sprintf " %s%s" "+" "]") +];

	para_index = [+ (sprintf "%s%s " "[" "+")  +](sum (for-index "[+ cat_name +]") 1)[+ (sprintf " %s%s" "+" "]") +];
	
	sendto_dest = [+ (sprintf "%s%s " "[" "+")  +](get "sendto_dest")[+ (sprintf " %s%s" "+" "]") +];
	
	sendto_flag = [+ (sprintf "%s%s " "[" "+")  +](get "sendto_block")[+ (sprintf " %s%s" "+" "]") +];
	
	argument_class = [+ (sprintf "%s%s " "[" "+")  +](get "argument_class")[+ (sprintf " %s%s" "+" "]") +];

[+ IF (exist?"cat_descript") +]	cat_descript = "[+ (get "cat_descript") +]";[+ ENDIF +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"para_descript"[+ (sprintf ") %s%s" "+" "]")  +]	para_descript = "[+ (sprintf "%s%s " "[" "+")  +](get "para_descript")[+ (sprintf " %s%s" "+" "]")  +]";[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " FOR ")  +]value_explain[+ (sprintf " %s%s" "+" "]")  +]	value_explain = "[+ (sprintf "%s%s " "[" "+")  +](get "value_explain")[+ (sprintf " %s%s" "+" "]")  +]";
[+ (sprintf "%s%s%s" "[" "+" " ENDFOR ")  +]value_explain[+ (sprintf " %s%s" "+" "]")  +]


[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"remotecall"[+ (sprintf ") %s%s" "+" "]")  +]
[+ COMMENT ===================== BEGIN ONLY FOR REMOTECALL ======================= +]
[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"rt_in_type"[+ (sprintf ") %s%s" "+" "]")  +][+ (sprintf "%s%s%s " "[" "+" " IF ")  +](string-length (get "rt_in_type"))[+ (sprintf " %s%s" "+" "]")  +]	rt_in_type = [+ (sprintf "%s%s " "[" "+")  +](get "rt_in_type")[+ (sprintf " %s%s" "+" "]")  +];[+ (sprintf "%s%s%s" "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s " "[" "+")  +](error (sprintf "rt_in_type for %s not specified!" (get "para_name")) )[+ (sprintf " %s%s" "+" "]") +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"rt_out_type"[+ (sprintf ") %s%s" "+" "]")  +][+ (sprintf "%s%s%s " "[" "+" " IF ")  +](string-length (get "rt_out_type"))[+ (sprintf " %s%s" "+" "]")  +]	rt_out_type = [+ (sprintf "%s%s " "[" "+")  +](get "rt_out_type")[+ (sprintf " %s%s" "+" "]")  +];[+ (sprintf "%s%s%s" "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s " "[" "+")  +](error (sprintf "rt_out_type for %s not specified!" (get "para_name")) )[+ (sprintf " %s%s" "+" "]") +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ COMMENT ===================== END ONLY FOR REMOTECALL ======================= +]
[+ (sprintf "%s%s%s " "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ COMMENT ===================== BEGIN ONLY FOR "NOT" REMOTECALL ======================= +]
[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"no_cache"[+ (sprintf ") %s%s" "+" "]")  +] no_cache;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" " +" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"var_type"[+ (sprintf ") %s%s" "+" "]")  +][+ (sprintf "%s%s%s " "[" "+" " IF ")  +](string-length (get "var_type"))[+ (sprintf " %s%s" "+" "]")  +]	var_type = [+ (sprintf "%s%s " "[" "+")  +](get "var_type")[+ (sprintf " %s%s" "+" "]")  +];[+ (sprintf "%s%s%s" "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s " "[" "+")  +](error (sprintf "var_type for %s not specified!" (get "para_name")) )[+ (sprintf " %s%s" "+" "]") +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ (sprintf "%s%s%s " "[" "+" " ELIF (exist?")  +]"int_type"[+ (sprintf ") %s%s" "+" "]")  +]	int_type;
[+ (sprintf "%s%s%s " "[" "+" " ELIF (exist?")  +]"uint_type"[+ (sprintf ") %s%s" "+" "]")  +]	uint_type[+ (sprintf "%s%s%s " "[" "+" " IF ")  +](string-length (get "uint_type"))[+ (sprintf " %s%s" "+" "]")  +] = [+ (sprintf "%s%s " "[" "+")  +](get "uint_type")[+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +];
[+ (sprintf "%s%s%s " "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s " "[" "+")  +](error (sprintf "%s type not specified" (get "para_name")))[+ (sprintf " %s%s" "+" "]")  +][+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"rw"[+ (sprintf ") %s%s" "+" "]")  +]rw;[+ (sprintf "%s%s%s " "[" "+" " ELIF (exist?")  +]"ro"[+ (sprintf ") %s%s" "+" "]")  +]ro;[+ (sprintf "%s%s%s " "[" "+" " ELIF (exist?")  +]"wo"[+ (sprintf ") %s%s" "+" "]")  +]wo;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"no_init_brd"[+ (sprintf ") %s%s" "+" "]")  +]no_init_brd;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"no_run_brd"[+ (sprintf ") %s%s" "+" "]")  +]no_run_brd;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"defaultval"[+ (sprintf ") %s%s" "+" "]")  +]defaultval = '[+ (sprintf "%s%s " "[" "+")  +](get "defaultval")[+ (sprintf " %s%s" "+" "]")  +]';[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"default"[+ (sprintf ") %s%s" "+" "]")  +]default;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"factory"[+ (sprintf ") %s%s" "+" "]")  +]factory;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"argument_count"[+ (sprintf ") %s%s" "+" "]")  +]	argument_count = "[+ (sprintf "%s%s " "[" "+")  +](get "argument_count")[+ (sprintf " %s%s" "+" "]")  +]";
[+ (sprintf "%s%s%s" "[" "+" " ELSE")  +][+ (sprintf " %s%s" "+" "]")  +]	argument_count = 1;
[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ COMMENT ===================== END ONLY FOR "NOT" REMOTECALL ======================= +]
[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ COMMENT ===================== BEGIN FOR ALL TYPE ======================= +]
[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"ui"[+ (sprintf ") %s%s" "+" "]")  +]	ui;[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]

[+ (sprintf "%s%s%s " "[" "+" " IF (exist?")  +]"uid"[+ (sprintf ") %s%s" "+" "]")  +]uid = [+ (sprintf "%s%s " "[" "+")  +](get "uid")[+ (sprintf " %s%s" "+" "]")  +];[+ (sprintf "%s%s%s" "[" "+" " ENDIF")  +][+ (sprintf " %s%s" "+" "]")  +]
[+ COMMENT ===================== END FOR ALL TYPE ======================= +]
};
[+ (sprintf "%s%s%s " "[" "+" " ENDFOR")  +][+ cat_name +][+  (sprintf " %s%s" "+" "]") +][+ ENDFOR category +][+ FOR category +]


para_indexes = {
	count = [+ (sprintf "%s%s%s " "[" "+" " (count ")  +]"[+ cat_name +]"[+ (sprintf ") %s%s" "+" "]")  +];
	cat_name = [+ cat_name +];
};[+ ENDFOR category +]

[+ == ddf +]autogen definitions parameter.tpl;
[+ INCLUDE "parameter_internal.tmp" +]
[+ ESAC +]

