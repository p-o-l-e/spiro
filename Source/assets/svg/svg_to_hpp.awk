BEGIN {
  print "#pragma once\n"
  print "#include \"descriptor.hxx\""
  print "#include <string>"
  print "#include <string_view>\n"
  print "namespace core\n{"

  print "\t/**********************************************************************************************************************"
  print "\t* "
  print "\t*  Descriptor"
  print "\t* "
  print "\t**********************************************************************************************************************/"
  print "\tnamespace " ns "\n\t{"
}

/<metadata/ {
  if (match($0, /data-ic="([0-9]+)"/, m)) ic=m[1]; else ic=0
  if (match($0, /data-oc="([0-9]+)"/, m)) oc=m[1]; else oc=0
  if (match($0, /data-cc="([0-9]+)"/, m)) cc=m[1]; else cc=0
  if (match($0, /data-prefix="([^"]+)"/, m)) prefix=m[1]; else prefix=ns
  if (match($0, /data-module="([^"]+)"/, m)) module=m[1]; else module=ns
  print "\t\tconstexpr int cc { " cc " };"
  print "\t\tconstexpr int ic { " ic " };"
  print "\t\tconstexpr int oc { " oc " };\n"
  print "\t\tconstexpr std::string prefix { \"" prefix "\" };\n"
}

/<rect/ {
    if( match($0, /x="([0-9.]+)"/, mx) &&
        match($0, /y="([0-9.]+)"/, my) &&
        match($0, /width="([0-9.]+)"/, mw) &&
        match($0, /height="([0-9.]+)"/, mh) &&
        match($0, /id="([^"]+)"/, mid)) 
    {
        rects[++n] = sprintf("\t\t{ Control::type::button, { %8.3f, %8.3f, %8.3f, %8.3f },, \"%s\", 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0x00, false, 0x00000000 },",
                         mx[1], my[1], mw[1], mh[1], mid[1])
    }
}

END {
  print "\n\tconstexpr core::Control set_c[cc] {"
  for (i=1; i<=n; i++) print rects[i]
  print "    };\n"
  print "    constexpr Rectangle<float> constrain { 0.0f, 0.0f, 100.0f, 100.0f };"
  print "    constexpr core::Descriptor descriptor {"
  print "      core::map::module::type::" module ","
  print "      { &ic, &oc, &cc },"
  print "      &prefix,"
  print "      { nullptr, nullptr, set_c },"
  print "      &constrain"
  print "    };"
  print "  }"
}
