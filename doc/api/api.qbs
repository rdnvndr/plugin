import "../main.qbs" as DocProduct

DocProduct {
    name: "plugin-api"
    type: "docs"

    docName: "plugin-dev"
    Group {
        fileTagsFilter: ["docs"]
        qbs.install: true
        qbs.installDir: product.docPath
    }
}
