from things import Things
import random


class Light(Things):
    """
    This class represents Light sensor
    """
    def get_state(self):
        value = random.randint(0, 100)
        self.state = str(value)
        return value


