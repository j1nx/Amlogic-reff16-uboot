8726m_mid_refb14_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb14 amlogic m1
8726m_mid_refb14_config_help:
	@echo =======================================================================
	@echo The mark in board is "Hisense MID R1000"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb14_config_help
