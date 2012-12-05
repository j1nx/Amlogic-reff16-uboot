8726m_mid_refb21_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refb21 amlogic m1
8726m_mid_refb21_config_help:
	@echo =======================================================================
	@echo The mark in board is "refb21 MID config"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refb21_config_help
