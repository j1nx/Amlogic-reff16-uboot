m2_socket_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m2_socket_v1 amlogic m2
	
m2_socket_config_help:
	@echo =======================================================================
	@echo The mark in board is "m2_socket_v1"
	@echo config command: \"make $(@:%_help=%)\"
	
help:m2_socket_config_help
