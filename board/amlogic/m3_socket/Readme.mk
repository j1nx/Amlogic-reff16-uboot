m3_socket_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m3_socket  amlogic m3
m3_socket_config_help:
	@echo =======================================================================
	@echo The mark in board is "Meson board AML8726-M"
	@echo config command: \"make $(@:%_help=%)\"
help:m3_socket_config_help
