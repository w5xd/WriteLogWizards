@ if not exist ModuleTemplate\ mkdir ModuleTemplate
@ if exist symbol_table.txt del symbol_table.txt
@ set exe=".\ConvertToVsTemplate\x64\Release\ConvertToVsTemplate"
@for %%f in (ModifiedVszTemplates\*.*) do @(
	@for %%p in ("%%f") do @(
		if /i "%%~np" neq "ReadMe" @(
		@echo convert %%f
		@%exe% "%%f" ".\ModuleTemplate\%%~np%%~xp" -s symbol_table.txt
		)
	)
)
%exe% -s symbol_table.txt -cs
move /y CompositeSymbols.cs WL12ModuleItem\CppTemplates\WL12ModuleTemplate
