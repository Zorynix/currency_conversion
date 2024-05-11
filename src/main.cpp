#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "components/rate_manager/component.hpp"
#include "handlers/currencies/handler.hpp"
#include "handlers/rates/handler.hpp"
#include "handlers/update/handler.hpp"
#include "hello.hpp"
#include "userver/clients/dns/component.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/secdist/component.hpp"
#include "userver/storages/secdist/provider_component.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<components::rate_manager::Component>()
          .Append<userver::components::Secdist>()
          .Append<userver::components::DefaultSecdistProvider>()
          .Append<handlers::currencies::Handler>()
          .Append<handlers::rates::Handler>()
          .Append<handlers::update::Handler>()
          .Append<userver::components::Postgres>("postgres-db-1")
          .Append<userver::clients::dns::Component>();

  return userver::utils::DaemonMain(argc, argv, component_list);
}
