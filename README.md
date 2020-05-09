[![CircleCI](https://circleci.com/gh/oxidizing/sihl-example-issues.svg?style=svg&circle-token=1351b6152d0a4c49555dada65d80fa977159028c)](https://circleci.com/gh/oxidizing/sihl-example-issues)

# Issue Management App

This is a simple app that can be used to manage issues written in Fullstack Reason using the web framework [Sihl](https://github.com/oxidizing/sihl/) and [ReasonReact](https://reasonml.github.io/reason-react/).

You can check out the live demo here: [https://sihl-example-issues.oxidizing.io/](https://sihl-example-issues.oxidizing.io/)

![Screenshot](/images/screen3.png)

## Features

- Login & Register (thanks to [@sihl/users](https://www.npmjs.com/package/@sihl/users))
- Confirm email address
- Reset password
- Creation of boards
- Creation of issues on a board
- Mark issues as completed

## Description

- Admin UI for users, boards and issues
- Uses backend models in the react frontend to share logic
- Optimistic rendering with error handling
- Unit tests with jest thanks to testable business logic with reducers

This project uses the [@sihl/users](https://www.npmjs.com/package/@sihl/users) app for user management.

## Getting started

Start a local MariaDB instance. You can use `docker-compose -f docker-compose.test.yml up -d` to quickly spin up a MariaDB instance with the correct version.

Run `yarn test` to run all tests.

Run `yarn sihl start` to start the web server. The admin UI can be accessed at `http://localhost:3000/admin/login/`.

## Tutorial

Check out the [tutorial](https://github.com/oxidizing/sihl/wiki/Tutorial:-Issue-management-app) in the Wiki.

## Screenshots

![Screenshot](/images/screen1.png)
![Screenshot](/images/screen2.png)
![Screenshot](/images/screen4.png)

## License

Copyright (c) 2020 [Oxidizing Systems](https://oxidizing.io/)

Distributed under the MIT License.
