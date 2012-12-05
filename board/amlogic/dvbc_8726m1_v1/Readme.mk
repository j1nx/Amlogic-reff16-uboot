m1_dvbc_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson dvbc_8726m1_v1 amlogic m1
m1_dvbc_config_help:
	@echo =======================================================================
	@echo The mark in board is "DVBC_8726M1_V1.0"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_dvbc_config_help
