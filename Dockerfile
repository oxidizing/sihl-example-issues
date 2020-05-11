# Build the ReasonReact app in a Node container
FROM node:10.16.3 AS js-builder
WORKDIR /home/sihl/app
COPY package.json yarn.lock bsconfig.json ./
COPY src src
COPY static static
RUN yarn
RUN yarn build

# Build the Sihl backend in a OPAM/OCaml container
FROM ocaml/opam2:alpine-3.10 AS ocaml-builder
WORKDIR /home/opam/app
COPY sihl_example_issues.opam Makefile dune-project ./
COPY src src
RUN sudo chown -R opam .
RUN opam remote remove --all default && \
        opam remote add default https://opam.ocaml.org
RUN opam pin add -y -n sihl_example_issues . && \
        opam pin add -y -n sihl_core https://github.com/oxidizing/sihl.git\#0.0.21 && \
        opam pin add -y -n sihl_email https://github.com/oxidizing/sihl.git\#0.0.21 && \
        opam pin add -y -n sihl_user https://github.com/oxidizing/sihl.git\#0.0.21
RUN opam depext -y sihl_example_issues
RUN opam install --deps-only -y sihl_example_issues
RUN opam config exec -- make

# Copy over the binaries from stage 1 and 2 and install systems dependencies
FROM alpine:latest
WORKDIR /app
# TODO use opam depext |> file to automatically install all systems deps
# This was defined by opam depext: RUN apk --update add emacs-nox libffi-dev linux-headers m4 pcre-dev postgresql-dev
RUN apk --update add pcre-dev postgresql-dev
COPY --from=ocaml-builder /home/opam/app/_build/default/src/bin/Run.exe run.exe
COPY --from=js-builder /home/sihl/app/dist static
ENV SIHL_ENV production
CMD ["./run.exe", "start"]
