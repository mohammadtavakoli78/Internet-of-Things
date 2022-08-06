from things import Things
import random


class Lamp(Things):
    """
    This class represents Light sensor
    """

    def __init__(self, device_id):
        super().__init__("lamp", device_id)
        self.state = "on"

    def get_state(self):
        return self.state

    def update_state(self, state):
        self.state = state

