let name = "Issue Management App";
let namespace = "issues";

let routes = database => [
  Routes.GetBoardsByUser.endpoint(namespace, database),
  Routes.GetIssuesByBoard.endpoint(namespace, database),
  Routes.AddBoard.endpoint(namespace, database),
  Routes.AddIssue.endpoint(namespace, database),
  Routes.CompleteIssue.endpoint(namespace, database),
  Routes.AdminUi.Issues.endpoint(namespace, database),
  Routes.AdminUi.Boards.endpoint(namespace, database),
  Routes.Client.Asset.endpoint(),
  Routes.Client.App.endpoint(),
];

let app = () =>
  Sihl.App.Main.App.make(
    ~name,
    ~namespace,
    ~routes,
    ~migration=Migrations.MariaDb.make(~namespace),
    ~commands=[],
    ~configurationSchema=[],
  );
