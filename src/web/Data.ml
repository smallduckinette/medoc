
type document_t = int * string * CalendarLib.Date.t * int list

module SearchProps = 
struct
  let request_name = "select * from searchFiles($1, $2)"
end


module SearchByTagProps = 
struct
  let request_name = "select * from searchFilesByTag($1, $2)"
end

module SearchInput = 
  Types.Pair
    (Types.String)
    (Types.String)

module SearchOutput = 
  Types.Nuplet3
    (Types.Int)
    (Types.String)
    (Sql.SqlDate)

module SearchRequest = 
  Request.Request
    (SearchInput)
    (SearchOutput)
    (SearchProps)

module SearchByLatestRequest =
  Request.Request
    (Types.Pair(Types.Int)(Types.String))
    (SearchOutput)
    (struct let request_name = "select * from searchFilesByLatest($1, $2)" end)

module SearchByTagRequest = 
  Request.Request
    (SearchInput)
    (SearchOutput)
    (SearchByTagProps)

module FilesProps = 
struct
  let request_name = "select * from getFilesForDocument($1)"
end

module FilesInput = 
  Types.Singleton
    (Types.Int)

module FilesOutput = 
  Types.Singleton
    (Types.Int)

module FilesRequest = 
  Request.Request
    (FilesInput)
    (FilesOutput)
    (FilesProps)

let doSearch dbPool search_fun search_input = 
  List.map
    (fun (docid, title, date) -> (docid, title, date, (DbPool.run dbPool (fun dbConn -> FilesRequest.request dbConn prerr_endline docid))))
    (DbPool.run dbPool (fun dbConn -> search_fun dbConn prerr_endline search_input))
    
let search dbPool search_term key = 
  doSearch dbPool SearchRequest.request (search_term, key)
    
let search_by_latest dbPool limit key = 
  doSearch dbPool SearchByLatestRequest.request (limit, key)

let search_by_tag dbPool tag key = 
  doSearch dbPool SearchByTagRequest.request (tag, key)

(* Document *)
module DocumentInput = 
  Types.Pair
    (Types.Int)
    (Types.String)

module DocumentProps = 
struct
  let request_name = "select * from getDocument($1, $2)"
end

module DocumentRequest = 
  Request.Request
    (DocumentInput)
    (SearchOutput)
    (DocumentProps)

let document dbPool docid key = 
  let (docid, title, date) = DbPool.run dbPool (fun dbConn -> DocumentRequest.single_request dbConn prerr_endline (docid, key))
  and files = DbPool.run dbPool (fun dbConn -> FilesRequest.request dbConn prerr_endline docid) in
    (docid, title, date, files)

(* Login *)

module LoginProps = 
struct
  let request_name = "select * from checkUser($1, $2)"
end

module LoginInput = 
  Types.Pair
    (Types.String)
    (Types.String)

module LoginOutput = 
  Types.Singleton
    (Sql.SqlBool)

module LoginRequest = 
  Request.Request
    (LoginInput)
    (LoginOutput)
    (LoginProps)

let login dbPool login key = 
  DbPool.run dbPool (fun dbConn -> LoginRequest.single_request dbConn prerr_endline (login, key))

(* Create document *)

module CreateDocumentInput = 
  Types.Nuplet3
    (Types.String)
    (Sql.SqlDate)
    (Types.String)

module CreateDocumentRequest = 
  Request.Request
    (CreateDocumentInput)
    (Types.Empty)
    (struct let request_name = "select * from createDocument($1, $2, $3)" end)

let create_document dbPool title date key = 
  DbPool.run dbPool (fun dbConn -> CreateDocumentRequest.single_request dbConn prerr_endline (title, date, key))

(* Delete document *)

module DeleteDocumentRequest = 
  Request.Request
    (Types.Singleton(Types.Int))
    (Types.Empty)
    (struct let request_name = "select * from deleteDocument($1)" end)

let delete_document dbPool docId = 
  DbPool.run dbPool (fun dbConn -> DeleteDocumentRequest.single_request dbConn prerr_endline docId)
  

(* Tags *)

module AllTagsProps =
struct
  let request_name = "select * from getAllTags()"
end

module AllTagsRequest = 
  Request.Request
    (Types.Empty)
    (Types.Pair(Types.String)(Types.Int))
    (AllTagsProps)

let get_all_tags dbPool = 
  DbPool.run dbPool (fun dbConn -> AllTagsRequest.request dbConn prerr_endline ())

(* Tags for document *)
module GetTagsForDocRequest = 
  Request.Request
    (Types.Singleton(Types.Int))
    (Types.Singleton(Types.String))
    (struct let request_name = "select * from getTagsForDocument($1)" end)

module GetTagsNotForDocRequest = 
  Request.Request
    (Types.Singleton(Types.Int))
    (Types.Singleton(Types.String))
    (struct let request_name = "select * from getTagsNotForDocument($1)" end)

type document_tag = TagIn | TagOut

let get_tags_for_document dbPool document_tag docid = 
  match document_tag with
      TagIn -> DbPool.run dbPool (fun dbConn -> GetTagsForDocRequest.request dbConn prerr_endline docid)
    | TagOut -> DbPool.run dbPool (fun dbConn -> GetTagsNotForDocRequest.request dbConn prerr_endline docid)

(* Delete tag *)
module DeleteTag = 
  Request.Request
    (Types.Pair(Types.Int)(Types.String))
    (Types.Empty)
    (struct let request_name = "select * from deleteTag($1, $2)" end)

let delete_tag dbPool docid tag = 
  DbPool.run dbPool (fun dbConn -> DeleteTag.single_request dbConn prerr_endline (docid, tag))

(* Add tag *)
module AddTag = 
  Request.Request
    (Types.Pair(Types.Int)(Types.String))
    (Types.Empty)
    (struct let request_name = "select * from tagDocument($1, $2)" end)

let add_tag dbPool docid tag = 
  DbPool.run dbPool (fun dbConn -> AddTag.single_request dbConn prerr_endline (docid, tag))
    
module Stats = 
  Request.Request
    (Types.Empty)
    (Types.Nuplet3(Types.Int)(Types.Int)(Types.String))
    (struct let request_name = "select * from getStats()" end)
    
let get_stats dbPool = 
  DbPool.run dbPool (fun dbConn -> Stats.single_request dbConn prerr_endline ())

  
