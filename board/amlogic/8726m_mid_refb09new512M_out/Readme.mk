8726m_mid_refb09new512M_out_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb09new512M_out amlogic m1
8726m_mid_refb09new512M_out_config_help:
	@echo =======================================================================
	@echo The mark in board is "refb09 MID out for 512M ddr config"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb09new512M_out_config_help
