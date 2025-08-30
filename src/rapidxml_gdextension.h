#ifndef RAPIDXML_FOR_GODOT_H
#define RAPIDXML_FOR_GODOT_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <vector>
#include "rapidxml/rapidxml.hpp"

namespace godot {

class RapidXML : public RefCounted {
    GDCLASS(RapidXML, RefCounted)

private:
    rapidxml::xml_document<> xml_doc;
    std::vector<char> xml_buffer;
    String last_error; 
    rapidxml::xml_node<>* _find_node_by_path(const String& path) const;
    void _get_all_paths_recursive(rapidxml::xml_node<>* node, const String& current_path, PackedStringArray& paths) const;

protected:
    static void _bind_methods();

public:
    RapidXML();
    ~RapidXML();

    bool parse_string(const String &content);
    String get_node_value(const String &path) const;
    String get_node_attribute(const String &path, const String &attribute_name) const;
    void clear();
    String get_last_error() const;
    bool parse_file(const String &file_path);
    PackedStringArray get_child_nodes(const String &path) const;
    Dictionary get_node_attributes(const String &path) const;
    String get_root_node_name() const;
    PackedStringArray get_all_node_paths() const;
};

}

#endif // RAPIDXML_FOR_GODOT_H