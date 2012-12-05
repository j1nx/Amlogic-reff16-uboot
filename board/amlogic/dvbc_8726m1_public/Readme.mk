m1_dvbc_public_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson dvbc_8726m1_public amlogic m1
m1_dvbc_public_config_help:
	@echo =======================================================================
	@echo The mark in board is "DVBC_8726M1_V1.0 for public board"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_dvbc_public_config_help
