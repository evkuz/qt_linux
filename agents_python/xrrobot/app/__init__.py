import sys
import pathlib
module_path = pathlib.Path(__file__).parent.parent.parent.resolve()
sys.path.append(str(module_path))

import os
from iqrdevice import device, app

# создание экземпляра приложения
app.config.from_object(os.environ.get('FLASK_ENV') or 'config.DevelopementConfig')

from iqrdevice.utils.controllers import RosRun
rosrun = RosRun("rosrun.sh", "rosrunkill.sh")

# import views
from . import views

# Import robot features
from . import actions
from . import services
from . import nodes

import iqrdevice.services as iqrservices
import iqrdevice.actions as iqractions
import iqrdevice.nodes as iqrnodes

device.set_name("XRRobot")

# Configuring nodes
hascube_node = iqrnodes.HasCubeNode()
rosrun_node = iqrnodes.RosRunNode()
position_node = nodes.VehiclePositionNode()

# this is needed for connecting events with subscripers
mainbus = device.main_bus
rosrun.set_event_bus(device.main_bus) # all events will be written to this bus

mainbus.add_subscriber(rosrun.name + '/main', rosrun_node)
# next line is alternative way, i will add it directly to service
#mainbus.add_subscriber("services", hascube_node)
mainbus.add_subscriber("services", position_node)
mainbus.add_subscriber("actions", position_node)

# this is needed to show nodes in device state
device.register_node(rosrun_node)
device.register_node(hascube_node)
device.register_node(position_node)


# Configuring Actions
device.register_action(actions.MoveToAAction())
device.register_action(actions.MoveToBAction())


# Configuring Services
device.register_service(iqrservices.StopRosRunService(rosrun))
device.register_service(iqrservices.StartRosRunService(rosrun))
device.register_service(iqrservices.SetHasCubeService(hascube_node))
