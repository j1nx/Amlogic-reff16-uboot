m1_BGA372-M_SKT_V1_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson BGA372-M_SKT_V1 amlogic m1	
m1_BGA372-M_SKT_V1_config_help:
	@echo =======================================================================
	@echo The mark in board is "BGA372-M_SKT_V1"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_BGA372-M_SKT_V1_config_help
