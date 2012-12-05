m1_8726m_ref_config: unconfig
	$(MKCONFIG) $(@:_config=)  arm aml_meson m1_8726m_ref amlogic m1
m1_8726m_ref_config_help:
	@echo =======================================================================
	@echo The mark in board is "Meson board AML8726-M"
	@echo config command: \"make $(@:%_help=%)\"
help:m1_8726m_ref_config_help
