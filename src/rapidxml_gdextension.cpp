#include "rapidxml_gdextension.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/file_access.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

RapidXML::RapidXML() {}

RapidXML::~RapidXML() {}

void RapidXML::_bind_methods() {
    ClassDB::bind_method(D_METHOD("parse_string", "content"), &RapidXML::parse_string);
    ClassDB::bind_method(D_METHOD("get_node_value", "path"), &RapidXML::get_node_value);
    ClassDB::bind_method(D_METHOD("get_node_attribute", "path", "attribute_name"), &RapidXML::get_node_attribute);
    ClassDB::bind_method(D_METHOD("clear"), &RapidXML::clear);
    ClassDB::bind_method(D_METHOD("get_last_error"), &RapidXML::get_last_error);
    ClassDB::bind_method(D_METHOD("parse_file", "file_path"), &RapidXML::parse_file);
    ClassDB::bind_method(D_METHOD("get_child_nodes", "path"), &RapidXML::get_child_nodes);
    ClassDB::bind_method(D_METHOD("get_node_attributes", "path"), &RapidXML::get_node_attributes);
    ClassDB::bind_method(D_METHOD("get_root_node_name"), &RapidXML::get_root_node_name);
    ClassDB::bind_method(D_METHOD("get_all_node_paths"), &RapidXML::get_all_node_paths);
}

bool RapidXML::parse_string(const String &content) {
    clear();

    CharString utf8_str = content.utf8();
    xml_buffer.assign(utf8_str.get_data(), utf8_str.get_data() + utf8_str.length());
    xml_buffer.push_back('\0');

    try {
        xml_doc.parse<0>(xml_buffer.data());
    } catch (const rapidxml::parse_error &e) {
        last_error = String("Parse error: ") + e.what();
        UtilityFunctions::printerr("[RapidXML] ", last_error);
        clear();
        return false;
    } catch (...) {
        last_error = "An unknown error occurred during parsing.";
        UtilityFunctions::printerr("[RapidXML] ", last_error);
        clear();
        return false;
    }

    return true;
}

String RapidXML::get_last_error() const {
    return last_error;
}

rapidxml::xml_node<>* RapidXML::_find_node_by_path(const String& path) const {
    if (!xml_doc.first_node()) {
        return nullptr;
    }

    PackedStringArray parts = path.split("/");
    if (parts.is_empty()) {
        return nullptr;
    }
    
    rapidxml::xml_node<>* current_node = xml_doc.first_node(parts[0].utf8().get_data());
    
    for (int i = 1; i < parts.size() && current_node; ++i) {
        current_node = current_node->first_node(parts[i].utf8().get_data());
    }

    return current_node;
}

String RapidXML::get_node_value(const String &path) const {
    rapidxml::xml_node<>* node = _find_node_by_path(path);
    if (node) {
        return String(node->value());
    }
    return String();
}

String RapidXML::get_node_attribute(const String &path, const String &attribute_name) const {
    rapidxml::xml_node<>* node = _find_node_by_path(path);
    if (node) {
        rapidxml::xml_attribute<>* attr = node->first_attribute(attribute_name.utf8().get_data());
        if (attr) {
            return String(attr->value());
        }
    }
    return String();
}

void RapidXML::clear() {
    xml_doc.clear();      
    xml_buffer.clear();   
}

bool RapidXML::parse_file(const String &file_path) {
    if (!FileAccess::file_exists(file_path)) {
        last_error = "File not found: " + file_path;
        UtilityFunctions::printerr("[RapidXML] ", last_error);
        return false;
    }

    Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::READ);
    if (file.is_null()) {
        last_error = "Failed to open file: " + file_path;
        UtilityFunctions::printerr("[RapidXML] ", last_error);
        return false;
    }

    String content = file->get_as_text();
    file->close();

    return parse_string(content);
}

PackedStringArray RapidXML::get_child_nodes(const String &path) const {
    PackedStringArray children;

    rapidxml::xml_node<>* parent_node = _find_node_by_path(path);
    if (parent_node) {
        for (rapidxml::xml_node<>* child_node = parent_node->first_node(); 
             child_node; 
             child_node = child_node->next_sibling()) 
        {
            if(child_node->type() == rapidxml::node_element) {
                children.push_back(String(child_node->name()));
            }
        }
    }
    
    return children;
}

Dictionary RapidXML::get_node_attributes(const String &path) const {
    Dictionary attributes;

    rapidxml::xml_node<>* node = _find_node_by_path(path);
    if (node) {
        for (rapidxml::xml_attribute<>* attr = node->first_attribute();
             attr;
             attr = attr->next_attribute())
        {
            attributes[String(attr->name())] = String(attr->value());
        }
    }

    return attributes;
}

String RapidXML::get_root_node_name() const {
    rapidxml::xml_node<>* root_node = xml_doc.first_node();
    if (root_node) {
        return String(root_node->name());
    }
    return String();
}

PackedStringArray RapidXML::get_all_node_paths() const {
    PackedStringArray all_paths;
    rapidxml::xml_node<>* root_node = xml_doc.first_node();
    if (root_node) {
        _get_all_paths_recursive(root_node, String(root_node->name()), all_paths);
    }
    return all_paths;
}

void RapidXML::_get_all_paths_recursive(rapidxml::xml_node<>* node, const String& current_path, PackedStringArray& paths) const {
    paths.push_back(current_path);

    for (rapidxml::xml_node<>* child_node = node->first_node();
         child_node;
         child_node = child_node->next_sibling())
    {
        if (child_node->type() == rapidxml::node_element) {
            String child_path = current_path + String("/") + String(child_node->name());
            _get_all_paths_recursive(child_node, child_path, paths);
        }
    }
}