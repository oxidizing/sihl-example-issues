let name = "Issue Management App";

let namespace = "issues";

let config = () =>
  Sihl_core.Config.Schema.[
    string_(
      ~default="console",
      ~choices=["smtp", "console", "memory"],
      "EMAIL_BACKEND",
    ),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_HOST"),
    int_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_PORT"),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_AUTH_USERNAME"),
    string_(~required_if=("EMAIL_BACKEND", "smtp"), "SMTP_AUTH_PASSWORD"),
    bool_(~default=false, "SMTP_SECURE"),
    bool_(~default=false, "SMTP_POOL"),
  ];

let middlewares = () =>
  Handler.[
    Opium.Std.(
      middleware(
        Middleware.static(
          ~local_path="../../../dist/",
          ~uri_prefix="/assets",
          (),
        ),
      )
    ),
    GetBoardsByUser.handler,
    GetIssuesByBoard.handler,
    AddBoard.handler,
    AddIssue.handler,
    CompleteIssue.handler,
    AdminUi.Issues.handler,
    AdminUi.Boards.handler,
    Client.handler,
  ];

let migrations = () => {
  Migration.migration();
};

let repositories = () => {
  [Repository.clean];
};

let bind = () => [];

let commands = () => [];
