


a3_mbx_without_uart_config: unconfig
	$(MKCONFIG) $(@:_config=)  arc aml_meson a3_8626-xl-without-uart amlogic a3

a3_mbx_without_uart_config_help:
	@echo =======================================================================
	@echo The mark in board is "8626-xl-144-a3-ddr2-socket v1"
	@echo config command: \"make $(@:%_help=%)\"
	
help:a3_mbx_without_uart_config_help
