pan8006_socket_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson pan8006_socket amlogic m1
pan8006_socket_config_help:
	@echo =======================================================================
	@echo The mark in board is "PAN8006"
	@echo config command: \"make $(@:%_help=%)\"
help:pan8006_socket_config_help
