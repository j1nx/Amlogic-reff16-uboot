8726m_mid_refb09new512M_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb09new512M amlogic m1
8726m_mid_refb09new512M_config_help:
	@echo =======================================================================
	@echo The mark in board is "refb09 MID for 512M ddr config"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb09new512M_config_help
