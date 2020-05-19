open Base;
let ( let* ) = Lwt.bind;

module GetBoardsByUser = {
  open Sihl.Http;
  [@deriving yojson]
  type body_out = list(Model.Board.t);

  let handler =
    get("/issues/users/:id/boards/", req => {
      let user_id = param(req, "id");
      let user = Sihl_user.Middleware.Authn.authenticate(req);
      let* response = Service.Board.get_all_by_user(req, user, ~user_id);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Response.json
      |> Lwt.return;
    });
};

module GetIssuesByBoard = {
  open Sihl.Http;
  [@deriving yojson]
  type body_out = list(Model.Issue.t);

  let handler =
    get("/issues/boards/:id/issues/", req => {
      let board_id = param(req, "id");
      let user = Sihl_user.Middleware.Authn.authenticate(req);
      let* response = Service.Issue.get_all_by_board(req, user, ~board_id);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Response.json
      |> Lwt.return;
    });
};

module AddBoard = {
  open Sihl.Http;
  [@deriving yojson]
  type body_in = {title: string};

  [@deriving yojson]
  type body_out = Model.Board.t;

  let handler =
    post("/issues/boards/", req => {
      let user = Sihl_user.Middleware.Authn.authenticate(req);
      let* {title} = require_body_exn(req, body_in_of_yojson);
      let* response = Service.Board.create(req, user, ~title);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Response.json
      |> Lwt.return;
    });
};

module AddIssue = {
  open Sihl.Http;
  [@deriving yojson]
  type body_in = {
    title: string,
    description: option(string),
    board: string,
  };

  [@deriving yojson]
  type body_out = Model.Issue.t;

  let handler =
    post("/issues/issues/", req => {
      let user = Sihl_user.Middleware.Authn.authenticate(req);
      let* {title, description, board: board_id} =
        require_body_exn(req, body_in_of_yojson);
      let* response =
        Service.Issue.create(req, user, ~title, ~description, ~board_id);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Response.json
      |> Lwt.return;
    });
};

module CompleteIssue = {
  open Sihl.Http;
  [@deriving yojson]
  type body_out = Model.Issue.t;

  let handler =
    post("/issues/issues/:id/complete/", req => {
      let issue_id = param(req, "id");
      let user = Sihl_user.Middleware.Authn.authenticate(req);
      let* response = Service.Issue.complete(req, user, ~issue_id);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Response.json
      |> Lwt.return;
    });
};

module Client = {
  let app_handler =
    Opium.App.get("/app/**", _ => {
      let dir =
        Sihl.Core.Config.read_string(~default="./static", "STATIC_FILES_DIR");
      let* resp =
        Cohttp_lwt_unix.Server.respond_file(~fname=dir ++ "/index.html", ());
      Lwt.return @@ Opium.Std.Response.of_response_body(resp);
    });

  let root_handler =
    Opium.App.get("/", _ => {
      let dir =
        Sihl.Core.Config.read_string(~default="./static", "STATIC_FILES_DIR");
      let* resp =
        Cohttp_lwt_unix.Server.respond_file(~fname=dir ++ "/index.html", ());
      Lwt.return @@ Opium.Std.Response.of_response_body(resp);
    });
};

module AdminUi = {
  module Issues = {
    open Sihl.Http;
    let handler = get("", _ => Response.empty |> Lwt.return);
    /* let endpoint = (root, database) => */
    /*   Sihl.App.Http.dbEndpoint({ */
    /*     database, */
    /*     verb: GET, */
    /*     path: {j|/admin/$root/issues/|j}, */
    /*     handler: (conn, req) => { */
    /*       open! Sihl.App.Http.Endpoint; */
    /*       let%Async token = */
    /*         Sihl.App.Http.requireSessionCookie(req, "/admin/login/"); */
    /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
    /*       let%Async issues = Service.Issue.getAll((conn, user)); */
    /*       let issues = issues |> Sihl.Common.Db.Result.Query.rows; */
    /*       Async.async @@ */
    /*       OkHtml(Sihl.Users.AdminUi.render(<AdminUi.Issues issues />)); */
    /*     }, */
    /*   }); */
  };

  module Boards = {
    open Sihl.Http;
    let handler = get("", _ => Response.empty |> Lwt.return);
    /* let endpoint = (root, database) => */
    /*   Sihl.App.Http.dbEndpoint({ */
    /*     database, */
    /*     verb: GET, */
    /*     path: {j|/admin/$root/boards/|j}, */
    /*     handler: (conn, req) => { */
    /*       open! Sihl.App.Http.Endpoint; */
    /*       let%Async token = */
    /*         Sihl.App.Http.requireSessionCookie(req, "/admin/login/"); */
    /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
    /*       let%Async boards = Service.Board.getAll((conn, user)); */
    /*       let boards = boards |> Sihl.Common.Db.Result.Query.rows; */
    /*       Async.async @@ */
    /*       OkHtml(Sihl.Users.AdminUi.render(<AdminUi.Boards boards />)); */
    /*     }, */
    /*   }); */
  };
};
