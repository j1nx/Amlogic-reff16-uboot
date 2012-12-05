m1_8726m_mid_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml8726m_mid amlogic m1
m1_8726m_mid_config_help:
	@echo =======================================================================
	@echo The mark in board is "MID"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_8726m_mid_config_help
