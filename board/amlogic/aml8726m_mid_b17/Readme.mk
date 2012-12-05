m1_8726m_mid_b17_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson aml8726m_mid_b17 amlogic m1
m1_8726m_mid_b17_config_help:
	@echo =======================================================================
	@echo The mark in board is "DPF_B17"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_8726m_mid_b17_config_help
