8726m_mid_refb12_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb12 amlogic m1
8726m_mid_refb12_config_help:
	@echo =======================================================================
	@echo The mark in board is "RM W12"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb12_config_help

