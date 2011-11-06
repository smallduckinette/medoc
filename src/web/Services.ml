open Eliom_parameters

let medoc_empty = Eliom_services.new_service ["latest"] unit ()
let medoc_search = Eliom_services.new_service ["search"] (string "term") ()
let medoc_view = Eliom_services.new_service ["view"] (int "docid") ()
let medoc_view_tag = Eliom_services.new_service ["tag"] (string "tag") ()
let medoc_login = Eliom_services.new_post_service ~fallback:medoc_empty ~post_params:(string "login" ** string "password") ()
let medoc_delete_doc = Eliom_services.new_service ["delete"](int "docId") ()
let medoc_delete_tag = Eliom_services.new_service ["deleteTag"](int "docId" ** string "tag") ()
let medoc_add_tag = Eliom_services.new_service ["addTag"](int "docId" ** string "tag") ()
