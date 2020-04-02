module Async = Sihl.Common.Async;

module Board = ClientBoardPage;
module Register = ClientRegisterPage;
module Login = ClientLoginPage;
module PasswordReset = ClientPasswordResetPage;
module NotFound = ClientNotFound;
module ConfirmEmail = ClientConfirmEmailPage;

module Route = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    | ["app", "login"]
    | [] =>
      <ClientLayout.ForwardIfLoggedIn url="/app/boards/">
        <Login />
      </ClientLayout.ForwardIfLoggedIn>
    | ["app", "password-reset"] =>
      <ClientLayout.ForwardIfLoggedIn url="/app/boards/">
        <PasswordReset />
      </ClientLayout.ForwardIfLoggedIn>
    | ["app", "confirm-email"] => <ConfirmEmail />
    | ["app", "register"] =>
      <ClientLayout.ForwardIfLoggedIn url="/app/boards/">
        <Register />
      </ClientLayout.ForwardIfLoggedIn>
    | ["app", "boards", ..._] => <Board />
    | _ => <NotFound />
    };
  };
};

module Main = {
  [@react.component]
  let make = (~children) => {
    let (error, setError) = React.useState(_ => None);
    let (message, setMessage) = React.useState(_ => None);

    <ClientContextProvider.Message value=(message, setMessage)>
      <ClientContextProvider.Error value=(error, setError)>
        <ClientNotification.Error />
        <ClientNotification.Message />
        children
      </ClientContextProvider.Error>
    </ClientContextProvider.Message>;
  };
};

ReactDOMRe.renderToElementWithId(<Main> <Route /> </Main>, "app");
