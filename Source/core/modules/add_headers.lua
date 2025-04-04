
local lfs = require("lfs") -- LuaFileSystem module

local function scan_folder_for_hpp(folder)
    local files = {}
    for file in lfs.dir(folder) do
        if file:match("%.hpp$") then
            table.insert(files, folder .. "/" .. file)
        end
    end
    return files
end

local function create_master_hpp(includes, output_file)
    local file = io.open(output_file, "w")
    if file then
        file:write("#pragma once\n\n")
        for _, include in ipairs(includes) do
            file:write("#include \"" .. include .. "\"\n")
        end
        file:close()
    else
        print("Failed to create the output file.")
    end
end

local folder_to_scan  = "interface" -- Replace with your folder path
local output_hpp_file = "interface_headers.hxx"

local hpp_files = scan_folder_for_hpp(folder_to_scan)
create_master_hpp(hpp_files, output_hpp_file)

print("Master .hxx file created successfully.")

