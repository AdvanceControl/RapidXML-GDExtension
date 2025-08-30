@tool
extends EditorPlugin


const ZH_CN = preload("res://addons/rapidxml_gdextension/translations/zh_CN.po")


const TR_FILES :Array[Translation]= [
	ZH_CN
	
]

func _enable_plugin() -> void:
	# Add autoloads here.
	pass


func _disable_plugin() -> void:
	# Remove autoloads here.
	pass


func _enter_tree() -> void:
	# Inside Godot domain
	var doc_domain := TranslationServer.get_or_add_domain("godot.documentation")
	for file in TR_FILES:
		doc_domain.add_translation(file)
	

func _exit_tree() -> void:
	var doc_domain := TranslationServer.get_or_add_domain("godot.documentation")
	for file in TR_FILES:
		doc_domain.remove_translation(file)
