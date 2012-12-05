8726m_mid_refc08_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson 8726m_mid_refc08 amlogic m1
8726m_mid_refc08_config_help:
	@echo =======================================================================
	@echo The mark in board is "refc08"
	@echo config command: \"make $(@:%_help=%)\"
help:8726m_mid_refc08_config_help
