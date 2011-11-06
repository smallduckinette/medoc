open Lwt
open XHTML.M
open Eliom_services
open Eliom_parameters
open Eliom_predefmod.Xhtml
open Common
open Services


let search_form = 
  fun search_term -> 
    [div[
       Eliom_predefmod.Xhtml.string_input ~a:[a_id "search_box"] ~input_type:`Text ~name:search_term ();
       Eliom_predefmod.Xhtml.string_input ~a:[a_id "search_button"] ~input_type:`Submit ~value:"Search" ()]]

let login_form = 
  fun (login, password) -> 
    [div[
       pcdata "Login";
       Eliom_predefmod.Xhtml.string_input ~input_type:`Text ~name:login ();
       pcdata "Password";
       Eliom_predefmod.Xhtml.string_input ~input_type:`Password ~name:password ();
       Eliom_predefmod.Xhtml.string_input ~input_type:`Submit ~value:"Login" ()]]
      
let image sp image alt = 
  img ~src:(make_uri ~service:(static_dir sp) ~sp [image]) ~alt ()
    
let dynimage sp fileid alt = 
  a DbImage.medoc_file sp [img ~src:(make_uri ~service:(DbImage.medoc_thumb) ~sp fileid) ~alt ()] fileid

let dynimages sp files = 
  List.map
    (fun fileid -> dynimage sp fileid (string_of_int fileid))
    files

let medoc_head sp = 
  (head (title (pcdata "Medoc documentation system")) [css_link ~uri:(make_uri ~service:(static_dir sp) ~sp ["style.css"]) ()])
    
let get_tags_for_document tagType service tag_to_service sp docid =
  let tags = Data.get_tags_for_document dbPool tagType docid in
    match tags with
	[] -> pcdata ""
      | t::q -> ul (li [a ~a:[a_class ["minitag"]] ~service ~sp [pcdata t] (tag_to_service t)]) (List.map (fun t -> li [a ~a:[a_class ["minitag"]] ~service ~sp [pcdata t] (tag_to_service t)]) q)

let data_to_row sp (docid, title, date, files) =
  tr (td ((a medoc_view sp [pcdata title] docid)::[get_tags_for_document Data.TagIn medoc_view_tag (fun tag -> tag) sp docid])) [td [pcdata (CalendarLib.Printer.Date.sprint "%d %b %Y" date)]; td (dynimages sp files)]

let data_to_rows sp data = 
  List.map (data_to_row sp) data

let title_row = tr (th [pcdata "Document"]) [th [pcdata "Date"]; th [pcdata "Pages"]]

let search_table data sp = 
  table title_row (data_to_rows sp data)
  
let custom_tag sp docid = 
  Eliom_predefmod.Xhtml.get_form medoc_add_tag sp (
    fun (form_docid, customTag) -> 
      [div ~a:[a_class ["inline"]] [pcdata "Custom:"; Eliom_predefmod.Xhtml.int_input ~input_type:`Hidden ~name:form_docid ~value:docid ();
	   Eliom_predefmod.Xhtml.string_input ~input_type:`Text ~name:customTag ()]])
	
let view_document sp docid key = 
  let (_, title, date, files) = Data.document dbPool docid key in
  let title_section = div [pcdata (title ^ " - " ^ (CalendarLib.Printer.Date.sprint "%d %b %Y" date))]
  and document_section = div (List.map (fun f -> dynimage sp f "file") files)
  and tags_section = div ~a:[a_id "view_tags"] [get_tags_for_document Data.TagIn medoc_view_tag (fun tag -> tag) sp docid]
  and add_tags_section = div ~a:[a_class ["subzone"]] [pcdata "Add tag"; get_tags_for_document Data.TagOut medoc_add_tag (fun tag -> (docid, tag)) sp docid; custom_tag sp docid]
  and delete_tags_section = div ~a:[a_class ["subzone"]] [pcdata "Remove tag"; get_tags_for_document Data.TagIn medoc_delete_tag (fun tag -> (docid, tag)) sp docid]
  in
    div ~a:[a_id "doctitle"]
      [title_section; 
       document_section; 
       tags_section;
       add_tags_section;
       delete_tags_section]

let doc_menu docid = 
  fun deleteDoc -> 
    [div [Eliom_predefmod.Xhtml.int_input ~input_type:`Hidden ~name:deleteDoc ~value:docid ();
	  Eliom_predefmod.Xhtml.string_input ~input_type:`Submit ~value:"Delete" ()]]
      
type action = 
    Search of string
  | View of int
  | ViewTag of string
  | NoAction

type operation = 
    Login of string * string
  | DeleteDoc of int
  | DeleteTag of int * string
  | AddTag of int * string
  | NoOperation

let medoc_main_zone action sp (login, key) = 
  match action with
      Search search_term -> div ~a:[a_id "main"; a_class ["zone"]] [search_table (Data.search dbPool search_term key) sp]
    | ViewTag tag -> div ~a:[a_id "main"; a_class ["zone"]] [search_table (Data.search_by_tag dbPool tag key) sp]
    | View docid -> div ~a:[a_id "main"; a_class ["zone"]] [view_document sp docid key]
    | NoAction -> div ~a:[a_id "main"; a_class ["zone"]] [search_table (Data.search_by_latest dbPool 10 key) sp]
	

let make_title_zone sp session_data action = 
  Lwt_preemptive.detach
    (fun () -> 
       let search_zone = 
	 match session_data with
	     Eliom_sessions.Data _ -> Eliom_predefmod.Xhtml.get_form medoc_search sp search_form
	   | _ -> pcdata ("")
       in
       let title_zone = match session_data with
	   Eliom_sessions.No_data 
	 | Eliom_sessions.Data_session_expired 
	   -> Eliom_predefmod.Xhtml.post_form medoc_login sp login_form ()
	 | Eliom_sessions.Data (login, _) 
	   -> pcdata ("Logged on as " ^ login)  
       in
	 div ~a:[a_id "header"]
	   [div ~a:[a_class ["header"]] [title_zone]; 
	    div ~a:[a_class ["header"]] [search_zone]]
       )
    ()
    
let make_logo_zone sp session_data action = 
  return (div ~a:[a_id "logo"] [image sp "logo.png" "logo"])

let make_main_zone sp session_data action = 
  Lwt_preemptive.detach
    (fun () -> 
	 match session_data with
	     Eliom_sessions.No_data 
	   | Eliom_sessions.Data_session_expired -> pcdata ("")
	   | Eliom_sessions.Data session_data -> medoc_main_zone action sp session_data)
    ()
    
let make_action_zone sp session_data action = 
  let create_document = li [a ~a:[a_class ["menu"; "tool"; "zone"]] ~service:medoc_empty ~sp [pcdata "Latest documents"] ()]
  in
  let do_make_action_zone sp action = 
    match action with
	View docid -> ul (create_document) []
      | _ -> ul (create_document) []
  in
    return
      (
	match session_data with
	    Eliom_sessions.No_data 
	  | Eliom_sessions.Data_session_expired -> pcdata ("")
	  | Eliom_sessions.Data _ -> do_make_action_zone sp action
      )
      
let make_tag_name tag count =
  tag ^ " (" ^ (string_of_int count) ^ ")"

let make_tag_list sp tags = 
  match tags with
      [] -> pcdata ""
    | (tag, count)::q -> 
	ul 
	  (li [a ~a:[a_class ["menu"; "tag"; "zone"]] ~service:medoc_view_tag ~sp [pcdata (make_tag_name tag count)] tag]) 
	  (List.map (fun (tag, count) -> li [a ~a:[a_class ["menu"; "tag"; "zone"]] ~service:medoc_view_tag ~sp [pcdata (make_tag_name tag count)] tag]) q)

let make_tag_zone sp session_data action = 
  Lwt_preemptive.detach
    (
      fun () -> 
	match session_data with 
	    Eliom_sessions.No_data 
	  | Eliom_sessions.Data_session_expired -> pcdata ("")
	  | Eliom_sessions.Data session_data -> 
	      let tag_list = Data.get_all_tags Common.dbPool in
		div [make_tag_list sp tag_list]
    )
    ()
    
let do_make_stats_zone sp (login, key) = 
  let (docs, files, size) = Data.get_stats dbPool in
  let message = (string_of_int docs) 
    ^ " documents, " 
    ^ (string_of_int files)
    ^ " files - "
    ^ size in
  div ~a:[a_id "stats"; a_class ["zone"]] [pcdata message]
    
let make_stats_zone sp session_data action = 
  Lwt_preemptive.detach
    (fun () -> 
      match session_data with
	  Eliom_sessions.No_data 
	| Eliom_sessions.Data_session_expired -> pcdata ("")
	| Eliom_sessions.Data session_data -> do_make_stats_zone sp session_data)
    ()
    
    
let medoc_main_page = 
  fun operation action sp () () -> 
    (match operation with
	 Login (login, key) -> 
	   if Data.login dbPool login key then 
	     Eliom_sessions.set_volatile_session_data session_table sp (login, key)
       | _ -> ()
    );
    let session_data = Eliom_sessions.get_volatile_session_data session_table sp () in
      (
	match (session_data, operation) with
	  | (Eliom_sessions.Data (_, key), DeleteDoc(docId)) -> Data.delete_document dbPool docId
	  | (Eliom_sessions.Data (_, _), DeleteTag(docid, tag)) -> Data.delete_tag dbPool docid tag
	  | (Eliom_sessions.Data (_, _), AddTag(docid, tag)) -> Data.add_tag dbPool docid tag
	  | _ -> ()
      );
      
    let page = 
      Lwt_util.fold_left 
	(fun acc t -> t >>= 
	   (fun r -> return (r::acc))) 
	[] 
	(List.rev[make_title_zone sp session_data action;
		  make_logo_zone sp session_data action;
		  make_action_zone sp session_data action;
		  make_tag_zone sp session_data action;
		  make_main_zone sp session_data action;
                  make_stats_zone sp session_data action])
    in
      page >>= 
	(fun p ->
	   return 
	     (html
		(medoc_head sp)
		(body [div ~a:[a_class ["mainframe"]] p]))
	)
	  
let _ = Eliom_predefmod.Xhtml.register medoc_search
  (fun sp search_term () -> medoc_main_page NoOperation (Search search_term) sp () ())
  
let _ = Eliom_predefmod.Xhtml.register medoc_view
  (fun sp docid () -> medoc_main_page NoOperation (View docid) sp () ())

let _ = Eliom_predefmod.Xhtml.register medoc_view_tag
  (fun sp tag () -> medoc_main_page NoOperation (ViewTag tag) sp () ())

let _ = Eliom_predefmod.Xhtml.register medoc_empty
  (medoc_main_page NoOperation NoAction)

let _ = Eliom_predefmod.Xhtml.register medoc_login
  (fun sp () (login, password) -> medoc_main_page (Login (login, password)) NoAction sp () ())

let _ = Eliom_predefmod.Xhtml.register medoc_delete_doc
  (fun sp docId () -> medoc_main_page (DeleteDoc(docId)) NoAction sp () ())

let _ = Eliom_predefmod.Xhtml.register medoc_delete_tag
  (fun sp (docid, tag) ()  -> medoc_main_page (DeleteTag (docid, tag)) (View docid) sp () ())

let _ = Eliom_predefmod.Xhtml.register medoc_add_tag
  (fun sp (docid, tag) ()  -> medoc_main_page (AddTag (docid, tag)) (View docid) sp () ())
