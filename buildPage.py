import htmlmin

data=""

# Read File
with open('include/page.html', 'r') as file:
    data=file.read()

# Minify HTML
data = htmlmin.minify(data.decode("utf-8"), remove_empty_space=True)

# Escape Quotes
data=data.replace('"','\\"')

# Write to Header File
with open('include/page.h', 'w') as file:
    file.write("#define PAGE_HTML \"" + data + "\"")
